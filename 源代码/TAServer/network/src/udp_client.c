#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<memory.h>
#include<stdio.h>
#include"u_msg.h"

int main(int argc,char *argv[]){
	if(argc<5){
		fprintf(stderr,"usage error:\n");
		exit(0);
	}
	int sockfd;
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0){
		perror("socket error");
		exit(1);
	}
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET,argv[1],&serveraddr.sin_addr.s_addr);
	Package p;
	struct sockaddr_in aimaddr;
	aimaddr.sin_family = AF_INET;
	aimaddr.sin_port = htons(atoi(argv[4]));
	inet_pton(AF_INET,argv[3],&aimaddr.sin_addr.s_addr);
	p.sockaddr = aimaddr;
	char buffer[1024]="Welcome hello world";
	char ip[16];
	int port;
	memset(ip,0,sizeof(ip));
	memcpy(p.buffer,buffer,strlen(buffer));
	while(1){
	if(sendto(sockfd,&p,sizeof(p),0,(struct sockaddr *)&serveraddr,sizeof(serveraddr))<0){
		perror("send error");
		exit(1);
	}else{
		if(recv(sockfd,&p,sizeof(p),0)<0){
			perror("recv error");
			exit(1);
		}else{
			inet_ntop(AF_INET,&p.sockaddr.sin_addr.s_addr,ip,sizeof(ip));
			port = ntohs(p.sockaddr.sin_port);
			printf("%s(%d):%s\n",ip,port,p.buffer);
		}
	}
	}
	return 0;
}
