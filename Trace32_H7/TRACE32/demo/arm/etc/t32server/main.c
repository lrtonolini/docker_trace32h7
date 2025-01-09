// --------------------------------------------------------------------------------
// @Title: t32server - main
// @Description:
// The t32server communicates with TRACE32 PowerView on the host wia DCC and with
// the gdbserver over Ethernet (localhost) allowing run mode debugging over DCC
// @Author: KJM
// @Copyright: (C) 1989-2022 Lauterbach GmbH, licensed for use with TRACE32(R) only
// --------------------------------------------------------------------------------
// $Id: main.c 18850 2022-01-26 18:41:29Z bschroefel $

#include "main.h"
#include "fileio.h"
#include "remote.h"
#include "arm-dcc.h"


#include <linux/auxvec.h>
#include <getopt.h>
#include <sched.h>
#include <ctype.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static pthread_t pmain;

static const char version[5] = "7.0";

static char gdbserver_path[100];
static char StrProcessStatus[BUFSIZE];

static int GdbServerFd = -1;

extern char serial_dcc;
extern int armfamily;
/* These are in <asm/elf.h> in current kernels.  */
#define HWCAP_VFP       64
#define HWCAP_IWMMXT    512
#define HWCAP_NEON      4096
#define HWCAP_VFPv3     8192
#define HWCAP_VFPv3D16  16384

static int nError = 0;
static int remote_debug = 0;

/* TASK.List variables */
static char P_cmd[16];
static char P_state;
static int P_pid;
static int P_ppid, P_pgrp;


void debugout(const char *format, ...)
{
	va_list arg;
	char emsg[4095];

	if (!remote_debug)
		return;

	va_start (arg, format);
	vsprintf(emsg, format, arg);
	va_end (arg);

	printf("%s", emsg);
}


/* open /proc/<pid>/stat and read the name (P_cmd), the pid (P_pid),
 * the state (P_state) the parent id (P_ppid) and the group id (P_pgrp)
 * of the process
 * from procpc-x.x/minimal.c
 */
static int stat2proc(int pid)
{
	char buf[800]; /* about 40 fields, 64-bit decimal is about 20 chars */
	int num;
	int fd;
	char * ptr;
	struct stat sb; /* stat() used to get EUID */

	snprintf(buf, 32, "/proc/%d/stat", pid);
	if ( (fd = open(buf, O_RDONLY, 0) ) == -1 ) return 0;
	num = read(fd, buf, sizeof buf - 1);
	fstat(fd, &sb);
	close(fd);
	if(num<80) return 0;
		buf[num] = '\0';
	ptr = strrchr(buf, ')');      /* split into "PID (cmd" and "<rest>" */
	*ptr = '\0';                  /* replace trailing ')' with NUL */
	/* parse these two strings separately, skipping the leading "(". */
	memset(P_cmd, 0, sizeof P_cmd);          /* clear */
	sscanf(buf, "%d (%15c", &P_pid, P_cmd);  /* comm[16] in kernel */
	num = sscanf(ptr + 2,                    /* skip space after ')' too */
		"%c "
		"%d %d ",
		&P_state,
		&P_ppid, &P_pgrp);

	if(P_pid != pid || P_pid == 0)
		return 0;
	ptr = strchr(P_cmd, ':');
	if (ptr != NULL) *ptr = ' ';
	ptr = strchr(P_cmd, ';');
	if (ptr != NULL) *ptr = ' ';
	ptr = strchr(P_cmd, ',');
	if (ptr != NULL) *ptr = ' ';
		return 1;
}

//! get the list of active processes by reading the /proc entries
static int GetTaskList(void)
{
	struct dirent *ent;          /* dirent handle */
	DIR *dir, *dir2;
	char process_string[50];
	char buf[100];
	int fd, i;
	char list[8000];
	char *p;

	dir = opendir("/proc");
	p  = list;

	while(( ent = readdir(dir) ))
	{
		if(*ent->d_name<'0' || *ent->d_name>'9') continue;
		if(!stat2proc(atoi(ent->d_name))) continue;
		if (P_state == 'Z')  /* Zombie Process */
		{
			char name[16];
			sprintf(name, "[%s]", P_cmd);
			sprintf(P_cmd, "%s", name);
		}

		/* get kernel processes (/proc/<pid>/cmdline empty) */
		snprintf(buf, 32, "/proc/%d/cmdline", P_pid);
		fd = open(buf, O_RDONLY, 0);
		if (fd < 0) continue;
		close(fd);

		sprintf(process_string, "%s:%d;", P_cmd, P_pid);
		for (i = 0; i< strlen(process_string); i++)
			*p++=process_string[i];

		if (P_pid == 1) continue;

		snprintf(buf, 32, "/proc/%d/task", P_pid);
		dir2 = opendir(buf);
		if (dir != NULL)
		{
			int ppid = P_pid;

			while (( ent = readdir(dir2) )) {
				if(*ent->d_name<'0' || *ent->d_name>'9') continue;
				if(!stat2proc(atoi(ent->d_name))) continue;
				if (ppid == P_pid) continue;
				sprintf(process_string, "%s:%d,%d;", P_cmd, ppid, P_pid);
				for (i = 0; i< strlen(process_string); i++)
					*p++=process_string[i];
			}
			closedir(dir2);
		}


	}
	closedir(dir);
	*p++='#';
	*p = '\0';
	TransmitBlock(list, strlen(list), 1000);
	return 0;
}

static int ProcessGdbCommand(char *cmdstr, int n)
{
	char reply[4096];
	int cnt;

	/* send the command to the gdbserver */
	if (send(GdbServerFd, cmdstr, strchr(cmdstr, '#') + 3 - cmdstr, 0) <= 0)
	{
		debugout("send error, errno = %i\n", errno);
		return -1;
	}
	/* get the reply now */
	cnt = GdbGetReply(reply, GdbServerFd, 500);

	if (cnt > 0)
		TransmitBlock(reply, strlen(reply), 1000);
	return 0;
}

//! check if a running process has stopped
static inline int CheckProcessStatus(void)
{
	int cnt;
	char reply[BUFSIZE];

	cnt = GdbGetReply(reply, GdbServerFd, 0);

	if (cnt <= 0)
		return -1;

	strcpy(StrProcessStatus, reply);
	TransmitBlock("&ST", 3, 10000);

	return 0;
}

static int ProcessT32Command(char *cmdstr)
{
	switch(cmdstr[1])
	{
		case 't': // get task list "&t%"
			GetTaskList();
			cmdstr[0] = '\0';
			break;

		case 'k': // system down &k%
			exit(0);

		case 'S':
			debugout("sending status: %s\n", StrProcessStatus);
			TransmitBlock(StrProcessStatus, strlen(StrProcessStatus), 10000);
			break;

		case 'f': /* file transfer */
			if (cmdstr[2] == 't')  /* get file from host */
				GetFile(&cmdstr[4]);
			else if(cmdstr[2] == 'r')
				SendFile(&cmdstr[4]);
			return 0;

		case 'd':
			remote_debug = !remote_debug;
			break;
	}
	return 0;
}

void mainloop(void)
{
	int n;
	char buf[4096];

	while (1)
	{
		buf[0] = 0;
		n = GetT32Cmd(buf);
		if (n != -1)
		{
			if (buf[0] == '$')  // forward a gdb command
				ProcessGdbCommand(buf, n);
			else if (buf[0] == '&') /* t32 commands */
				ProcessT32Command(buf);
		}
		CheckProcessStatus();
	}
}


#ifdef __ARM_EABI__
  static const char archstr[] = "ARM EABI over DCC";
#else
  static const char archstr[] = "ARM over DCC";
#endif


void t32server_version ()
{
	printf("t32server version %s\n", version);
	printf("Lauterbach GmbH 2014\n");
	printf("t32server was configured for %s\n", archstr);
}

void t32server_usage()
{
	printf("\tusage: t32server [OPTIONS]\n"
		"\tOPTIONS:\n"
		"\t\t--version: display the version of the t32server.\n"
		"\t\t--help: display the t32server usage.\n"
		"\t\t--gdbserver-path=<path>: location of your gdbserver\n"
		"\t\t--serial-dcc: use run mode and terminal over dcc.\n"
		"\n");
}

static int StartGdbServer(void)
{
	char connection[20], multi[10], *args[4];
	pid_t pid, pid2;

	strcpy(multi, "--multi");
	sprintf(connection, "127.0.0.1:%i", 2345+nError);
	args[0] = gdbserver_path;
	args[1] = multi;
	args[2] = connection;
	args[3] = NULL;

	pid = fork();
	if (pid<0)
		return -1;
	if (pid==0)
	{
		if (execv(gdbserver_path, args) < 0)
 			exit(1);
	}

	sleep(1);
	pid2 = waitpid(pid, NULL, WNOHANG);
	if (pid2 == pid || pid2 == -1)
	{
		printf("!!!error starting gdbserver!!!\n");
		return -1;
	}
	if ((GdbServerFd = GdbNetOpen(2345+nError)) < 0)
	{
		printf("!! t32server error: cannot connect to gdbserver (error %i) !!\n", errno);
		nError++;
		return -1;
	}
	return 0;
}

//! main routine
int main(int argc, char *argv[])
{
	int n;
	char **next_arg = &argv[1];
	struct stat sb;

	cpu_set_t cur_mask, new_mask;

	/* force the t32server to run on core 0 since TRACE32 only uses the DCC register of the first core */
	CPU_ZERO(&new_mask);
	CPU_SET(0, &new_mask);

	if (sched_setaffinity(0, sizeof (new_mask), &new_mask)) {
		printf("failed to set affinity.\n");
		goto done;
	}

	if (sched_getaffinity(0, sizeof (cur_mask), &cur_mask) < 0) {
		printf( "failed to get affinity.\n");
	}
done:

	sprintf(gdbserver_path, "/bin/gdbserver");
	while (*next_arg != NULL)
	{
		if (strcmp (*next_arg, "--version") == 0)
		{
			t32server_version ();
			exit (0);
		}
		else if (strcmp (*next_arg, "--help") == 0)
		{
			t32server_usage();
			exit (0);
		}
		else if (strncmp (*next_arg, "--gdbserver-path=", 17) == 0)
		{
			char *ptr;

			sprintf(gdbserver_path, "%s/gdbserver", *next_arg+17);
			ptr = strchr(gdbserver_path, ' ');
			if (ptr) *ptr = 0;
		}

		else if (strcmp (*next_arg, "--remote-debug") == 0)
			remote_debug = 1;
		else if (strcmp (*next_arg, "--serial-dcc") == 0)
		{
			serial_dcc = 1;
		}
		else if (strchr(*next_arg, ':'))
		{
			t32server_usage();
			exit (0);
		}
		else
		{
			fprintf (stderr, "Unknown argument: %s\n", *next_arg);
			exit (1);
		}
		next_arg++;
		continue;
	}
	/* check if localhost is configured */
	n = GdbNetOpen(2344);
	if (n == -3)
	{
		printf("please make sure that the address 127.0.0.1 is configured\n");
		printf("t32server exiting\n");
		return -1;
	}
	else if (n > 0)
		close(n);

	/* check if there is a gdbserver under /bin */
	if (stat(gdbserver_path, &sb) < 0)
	{
		printf("cannot find %s, t32server exiting\n", gdbserver_path);
		return -1;
	}

	/** everything OK so let's begin **/

	if (StartGdbServer() < 0)
		return -1;

	/* print the t32server version */
	printf("t32server version %s\n", version);
	if (serial_dcc)
	{
		if (init_dcc_console())
			serial_dcc = 0;
	}

	debugout("checking arm version\n");
	get_arm_version(); /* this functions stays in a loop until the arm version is defined (sent after Go.MONitor) */

	mainloop();

	return 0;
}
/* end of main.c */
