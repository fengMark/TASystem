#include<unistd.h>
#include<string.h>
#include<memory.h>
#include<sys/types.h>
#include"msg.h"

static  unsigned char msg_check(Msg *m){//定义协议收发双方的校验码
	unsigned char s = 0;
	int i;
	for(i=0;i<sizeof(m->head);i++){
		s+=m->head[i];
	}
	for(i=0;i<sizeof(m->buff);i++){
		s+=m->buff[i];
	}
	return s;
}
int write_msg(int sockfd,char *buff,size_t len){//自定义协议的发送
	Msg m;
	memset(&m,0,sizeof(m));
	strcpy(m.head,"markfeng");//协议头部
	memcpy(m.buff,buff,len);//协议的数据
	m.checknum=msg_check(&m);//生成协议的校验码
	if(write(sockfd,&m,sizeof(m))!=sizeof(m)){//系统调用发送自定义协议数据包
		return -1;
	}
}

int read_msg(int sockfd,char *buff,size_t len){//自定义协议的读取
	Msg m;
	memset(&m,0,sizeof(m));
	size_t size;
	if((size=read(sockfd,&m,sizeof(m)))<0){//读取整个协议数据包
		return -1;
	}else if(size==1){
		return 0;
	}
	unsigned char s = msg_check(&m);//校验码和协议头部验证数据包
	if((s==(unsigned char)m.checknum)&&(!strcmp("markfeng",m.head))){
		memcpy(buff,m.buff,len);//读取数据包内容
		return sizeof(m);//返回写入的字节大小
	}
	return -1;
}


