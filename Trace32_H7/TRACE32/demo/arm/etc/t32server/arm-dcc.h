// --------------------------------------------------------------------------------
// @Title: t32server - DCC functions
// @Description: -
// @Author: KJM
// @Copyright: (C) 1989-2022 Lauterbach GmbH, licensed for use with TRACE32(R) only
// --------------------------------------------------------------------------------
// $Id: arm-dcc.h 18850 2022-01-26 18:41:29Z bschroefel $


#ifdef __arm__
#ifndef ARMDCC_H_
#define ARMDCC_H_

#define TTY_BUF_SIZE 3072
#define TERMINAL_BLOCK_SIZE 3

int send_dcc(char *buf, int cnt, int timeout, int terminal);
int send_dcc_fast(int data1, int data2, int data3, int len);
int read_dcc(char *buf, int timeout);
int init_dcc_console();
void get_arm_version();

int get_dcc_status(int arm);
void write_dcc_reg(int arm, int data, int terminal);

#endif /*ARMDCC_H_*/
#endif /* __arm__ */
