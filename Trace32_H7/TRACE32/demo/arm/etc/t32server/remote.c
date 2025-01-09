// --------------------------------------------------------------------------------
// @Title: t32server - remote
// @Description: -
// @Author: KJM
// @Copyright: (C) 1989-2022 Lauterbach GmbH, licensed for use with TRACE32(R) only
// --------------------------------------------------------------------------------
// $Id: remote.c 18850 2022-01-26 18:41:29Z bschroefel $

#include "main.h"
#include "remote.h"
#include "arm-dcc.h"

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <stdlib.h>

/* seconds to wait for connect */
#define TIMEOUT 15
#define POLL_INTERVAL 2

extern void debugout(const char *format, ...);

//! TRACE32 -> t32server <- gdbserver's
static int ReceiveBlock(char *buffer, int len, int timeout, int fd)
{
	fd_set fds;
	int n, tmp, m;
	struct timeval tv;
	time_t t0;

	if ((t0 = time(0)) == -1)
	{
		return -4;
	}

	tv.tv_sec = 0;
	tv.tv_usec = timeout * 1000; // 500 ms
	n=0;
	while (n<len)
	{
		/* Check readability */
		FD_ZERO (&fds); FD_SET (fd, &fds);
		tmp = select (fd+1, &fds, (void *)0, (void *)0, &tv);
		/* Error. Return */
		if (tmp < 0)
		{
			return -1;
		}
		/* Data available. Read */
		else if ((tmp > 0) && FD_ISSET (fd, &fds))
		{
			if ((m = recv (fd, buffer+n, len - n,0)) <= 0)
			{
				return -2;
			}
			else n += m;
			buffer[n] = 0;
		} else
			break;
		/* Update time left. (0s < Error < 1s) */
		if ((tv.tv_sec -= (long) (difftime (time (0), t0) - timeout/1000.0)) < 0)
			tv.tv_sec = 0;
	}
	return n;
}

//! open a tcp socket and establish the connection
int GdbNetOpen(int port)
{
	struct sockaddr_in sockaddr;
	struct hostent *hostent;
	int n, fd, tmp, error;
	const char hostname[100] = {'1','2','7','.','0','.','0','.','1'};

	hostent = gethostbyname (hostname);
	if (!hostent)
	{
		fprintf (stderr, "%s: unknown host\n", hostname);
		return -1;
	}

	fd = socket (PF_INET, SOCK_STREAM, 0);

	if (fd < 0)
		return -1;

	sockaddr.sin_family = PF_INET;
	sockaddr.sin_port = htons(port);
	memcpy (&sockaddr.sin_addr.s_addr, hostent->h_addr, sizeof (struct in_addr));

	n = connect (fd, (struct sockaddr *) &sockaddr, sizeof (sockaddr));
	if (n < 0 && errno != EINPROGRESS)
	{
		error = errno;
		close(fd);
		if (error == 101)
		    return -3;
		return -2;
	}

	if (n)
	{
		/* looks like we need to wait for the connect */
		struct timeval t;
		fd_set rset, wset, eset;
		int polls = 0;
		fflush(stderr);
		FD_ZERO (&rset);

		do {
			FD_SET (fd, &rset);
			wset = rset;
			eset = rset;
			t.tv_sec = 0;
			t.tv_usec = 1000000 / POLL_INTERVAL;
			n = select (fd + 1, &rset, &wset, &eset, &t);
			polls++;

		} while (n == 0 && polls <= TIMEOUT * POLL_INTERVAL);

		if (n < 0 || polls > TIMEOUT * POLL_INTERVAL)
		{
			close (fd);
			fd = -1;
			return -2;
		}
	}

	/* Got something.  Is it an error? */
	{
		int res, err;
		socklen_t len;
		len = sizeof (err);
		res = getsockopt (fd, SOL_SOCKET, SO_ERROR, (void *) &err, &len);
		tmp = 1;
		// disable Nagle algorithm needed in some cases
		res = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *) &tmp, sizeof(tmp));
		if (res < 0 || err)
		{
			close (fd);
			fd = -1;
			return -1;
		}

	}

	tmp = 1;
	// disable Nagle algorithm needed in some cases
	setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *) &tmp, sizeof(tmp));

	return fd;
}

//! t32server -> TRACE32
int TransmitBlock(char *buf, int len, int timeout)
{
	int cnt = 0;

	if (len > 1)
		debugout("reply to host (%i): (\"%s\")\n", len, buf);
	cnt = send_dcc(buf, len, timeout, 0);

	if (cnt <= 0 && len > 1)
		debugout("TransmitBlock: error\n");
	return cnt;
}


int GdbGetReply(char *reply, int fd, int timeout /* in ms */)
{
	int cnt;
	int i = 0;

	memset(reply, 0, BUFSIZE);
	do {
		if (timeout == 0 && i > 0)
			timeout = 500; /* ms */
		cnt = ReceiveBlock(reply+i, 1, timeout, fd);
		if (cnt > 0)
			i += cnt;
	} while (cnt > 0 && reply[i - 1] != '#' && i < BUFSIZE);
		
	if (cnt > 0 && i < BUFSIZE)
		ReceiveBlock(reply+i, 2, timeout, fd);
	else
		return -1;
	reply[i+2] = 0;

	send(fd, "+", 1, 0);
	return i;
}

inline int GetT32CmdDCC(char *buf)
{
	int data, nmax = -1, timeout = 1000, len, n, i;
	char tmp[4096];

start:
	n = 0;

	while (1)
	{
		if (n == 0)
			timeout = 1;
		else
			timeout = 10000; /* we received a part of the packet, so wait more for the rest */

		data = read_dcc (tmp, timeout);
		if (data <= 0)
		{
			if (n > 0)
				return n;
			else
				return -1;
		}
		len = ((data >> 24) & 0x3) + 1;
		if (n == 0)
			debugout("[T32]: (\"");
		for(i=0; i<len; i++)
		{
			buf[n] = (data >> (8*i)) & 0x7f;
			debugout("%c", buf[n]);
			if ((buf[n] == '$' || buf[n] == '&') && n != 0)
			{
				/* new packet in the middle of an old one */
				buf[0] = buf[n];
				n = 0;
			}
			if (buf[n] == '%') /* end of t32 command */
			{
				debugout("\")\n");
				buf[n+1] = 0;
				return n;
			}
			if (buf[n] == '#') /* end of gdb command = n+2 (checsum) */
			{
				nmax = n + 2;
			}
			n++;
			if (n > nmax && nmax != -1)
			{
				buf[n] = '\0';
				debugout("\")\n");
				return n;
			}
		}
		if (n > 1024)
		{
			debugout("TIMEOUT\n");
			n = 0;
			goto start;
		}
	}
	return n;
}

int GetT32Cmd(char *buf)
{
	buf[0] = 0;
	return GetT32CmdDCC(buf);

}
