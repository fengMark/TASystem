#include<pthread.h>
#include<netdb.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<string.h>
#include<arpa/inet.h>
#include<signal.h>
#include<time.h>
#include"msg.h"
#include<errno.h>
#include<sys/wait.h>
int sockfd;
void sig_handler(int signo){
	if(signo==SIGINT){
		printf("server close\n");
		close(sockfd);
		exit(1);
	}
}
void do_service(int fd){
	char buff[512];
	while(1){
		memset(buff,0,sizeof(buff));
//		printf("start read and write ...\n");
		size_t size;
		if((size=read_msg(fd,buff,sizeof(buff)))<0){
			perror("read_msg error");
			break;
		}else if(size==0){//判断对方客户端是否已经关闭
			break;
		}else{
			printf("%s\n",buff);
			if(write_msg(fd,buff,sizeof(buff))<0){
				if(errno==EPIPE){//判断对方客户端是否已经关掉，客户端关掉的话则会产生EPIPE信号
					break;
				}
				perror("protocal error");
			}
		}
	}
}
void out_fd(int fd){
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	//从fd中获得客户端的相关信息
	if(getpeername(fd,(struct sockaddr*)&addr,&len)<0){
		perror("getpeername error");
		return;
	}
	char ip[16];
	memset(ip,0,sizeof(ip));
	int port =ntohs(addr.sin_port);
	inet_ntop(AF_INET,&addr.sin_addr.s_addr,ip,sizeof(ip));
	printf("%16s(%5d) closed\n",ip,port);
}
void * th_fn(void *arg){
	int fd =(int)arg;
	do_service(fd);
	out_fd(fd);
	close(fd);
	return (void*)0;
}
int main(int argc,char *argv[]){
	if(argc<2){
		printf("usage:%s #port\n",argv[0]);
		exit(1);
	}
	if(signal(SIGINT,sig_handler)==SIG_ERR){
		perror("signal sigint error");
		exit(1);
	}
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		perror("socket error");
		exit(1);
	}
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons((short)atoi(argv[1]));
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	if(bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
		perror("bind error");
		exit(1);
	}
	if(listen(sockfd,10)<0){
		perror("listen error");
		exit(1);
	}
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	while(1){
		//主控線程負責調用accept函數去獲得客戶端的連接
		int fd = accept(sockfd,NULL,NULL);
		if(fd<0){
			perror("accept error");
			continue;
		}
		//啓動子線程去調用io函數和連接的客戶端進行雙向通信
		pthread_t th;
		int err;
		//以分離狀態啓動子線程，子線程結束後會自動釋放資源
		if((err=pthread_create(&th,&attr,th_fn,(void*)fd))!=0){
			perror("pthread create error");
		}
		pthread_attr_destroy(&attr);
	}
	return 0;
}
