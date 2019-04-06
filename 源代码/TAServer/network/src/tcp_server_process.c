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
	if(signo==SIGCHLD){
		printf("child process deaded...\n");
		wait(NULL);
	}
}
void out_addr(struct sockaddr_in *clientaddr){
	int port = ntohs(clientaddr->sin_port);
	char ip[16];
	memset(ip,0,sizeof(sizeof(ip)));
	inet_ntop(AF_INET,&clientaddr->sin_addr.s_addr,ip,sizeof(ip));
	printf("client:%s(%d)connected\n",ip,port);
}
void do_service(int fd){
	char buff[512];
	while(1){
		memset(buff,0,sizeof(buff));
		printf("start read and write ...\n");
		size_t size;
		if((size=read_msg(fd,buff,sizeof(buff)))<0){
			perror("read_msg error");
			break;
		}else if(size==0){
			break;
		}else{
			printf("%s\n",buff);
			if(write_msg(fd,buff,sizeof(buff))<0){
				if(errno==EPIPE){
					break;
				}
				perror("protocal error");
			}
		}
	}
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
	
	if(signal(SIGCHLD,sig_handler)==SIG_ERR){
		perror("signal sigchld error");
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
	struct sockaddr_in clientaddr;
	socklen_t clientaddr_len=sizeof(clientaddr);
	while(1){
		int fd = accept(sockfd,(struct sockaddr*)&clientaddr,&clientaddr_len);
		if(fd<0){
			perror("accept error");
			continue;
		}
		pid_t pid;
		pid=fork();
		if(pid<0){
			continue;
		}else if(pid==0){	
			out_addr(&clientaddr);
			do_service(fd);
			close(fd);
			break;
		}else{
			close(fd);
		}
	}
	return 0;
}
