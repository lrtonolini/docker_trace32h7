// --------------------------------------------------------------------------------
// @Title: t32server - main
// @Description: -
// @Author: KJM
// @Copyright: (C) 1989-2022 Lauterbach GmbH, licensed for use with TRACE32(R) only
// --------------------------------------------------------------------------------
// $Id: main.h 18850 2022-01-26 18:41:29Z bschroefel $

#ifndef MAIN_
#define MAIN_

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>	/* main.c, remote.c */
#include <errno.h>	/* fileio.c, main.c, remote.c */

#include <string.h>	/* fileio.c, main.c, remote.c */
#include <fcntl.h>	/* fileio.c, main.c, thread.c */

#include <sys/socket.h>	/* main.c, included in remote.c */

#include <time.h>

#define BUFSIZE  4095

extern char serial_dcc;
extern int armfamily;


#endif /*MAIN_*/
