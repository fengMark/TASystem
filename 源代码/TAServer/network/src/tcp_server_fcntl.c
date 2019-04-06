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
#include"vector_fd.h"
//#include<errno.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<errno.h>
VectorFD *vfd;
int sockfd;
void sig_handler(int signo){
	if(signo==SIGINT){
		printf("server close\n");
		close(sockfd);
		destroy_vector_fd(vfd);
		exit(1);
	}
}
/*
fd对应与某个连接的客户端
和某一个连接的客户端进行双向通信

*/

void do_service(int fd){
	char buff[512];
	memset(buff,0,sizeof(buff));
	size_t size = read(fd,buff,sizeof(buff));
//因为采用非阻塞的方式，若读不到数据直接返回
//直接服务下一个客户端
//因此不需要判断size小于0的情况
	if(size==0){//客户端已经关闭连接
		char *buf = "client closed\n";
		write(STDOUT_FILENO,buf,sizeof(buf));
//从动态数组中删除对应的fd，关闭对应客户端的socket
		remove_fd(vfd,fd);
		close(fd);
	}else if(size>0){
		write(STDOUT_FILENO,buff,sizeof(buff));
		if(write(fd,buff,size)<0){
			if(errno==EPIPE){//判断对方客户端是否已经关掉，客户端关掉的话则会产生EPIPE信号
				perror("write error");
				remove_fd(vfd,fd);
				close(fd);
			}
		}
		
	}
	
}

void * th_fn(void *arg){
	int i;
	while(1){
		i=0;
		for(;i<vfd->counter;i++){
			do_service(get_fd(vfd,i));
		}
	}
	return (void*)0;
}
void out_addr(struct sockaddr_in *clientaddr){
	char ip[16];
	memset(ip,0,sizeof(ip));
	int port = ntohs(clientaddr->sin_port);
	inet_ntop(AF_INET,
		&clientaddr->sin_addr.s_addr,ip,sizeof(ip));
	printf("%s(%d) connected\n",ip,port);
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
//创建放置套接字描述副fd的动态数组
	vfd = create_vector_fd();
	pthread_t th;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	int err;
	if((err=pthread_create(&th,&attr,th_fn,(void*)0))!=0){
		perror("pthread create error");
		exit(1);
	}
	pthread_attr_destroy(&attr);
	/*
	*主控线程获得客户端的连接，将新的socket描述副
	*放置到动态数组中
	*启动的子线程负责遍历动态数组中的socket描述副
	*并和对应的客户端进行双向通信（采用非阻塞方式）
	*/
	struct sockaddr_in clientaddr;
	socklen_t clientaddr_len = sizeof(clientaddr);
	while(1){
		//主控線程負責調用accept函數去獲得客戶端的連接
		int fd = accept(sockfd,(struct sockaddr*)&clientaddr,
				&clientaddr_len);
		if(fd<0){
			perror("accept error");
			continue;
		}
		out_addr(&clientaddr);
		//该为非阻塞方式读写
		int val;
		fcntl(fd,F_GETFL,&val);
		val |= O_NONBLOCK;
		fcntl(fd,F_SETFL,&val);
		add_fd(vfd,fd);
	}
	return 0;
}
