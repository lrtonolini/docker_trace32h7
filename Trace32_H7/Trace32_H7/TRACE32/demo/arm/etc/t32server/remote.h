// --------------------------------------------------------------------------------
// @Title: t32server - remote
// @Description: -
// @Author: KJM
// @Copyright: (C) 1989-2022 Lauterbach GmbH, licensed for use with TRACE32(R) only
// --------------------------------------------------------------------------------
// $Id: remote.h 18850 2022-01-26 18:41:29Z bschroefel $

#ifndef REMOTE_H_
#define REMOTE_H_

int GdbNetOpen(int port);
int GdbGetReply(char *reply, int fd, int timeout);
int TransmitBlock(char *buf, int len, int timeout);
int GetT32Cmd(char *buf);


#endif /*REMOTE_H_*/
