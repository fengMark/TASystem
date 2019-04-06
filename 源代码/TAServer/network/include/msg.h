#ifndef __MSG_H_
#define __MSG_H_
#include<sys/types.h>
typedef struct {
	char head[10];//頭部
	char checknum;//校驗碼
	char buff[512];//數據
}Msg;

extern int write_msg(int sockfd,char *buff,size_t len);

extern int read_msg(int sockfd,char *buff,size_t len);

#endif
