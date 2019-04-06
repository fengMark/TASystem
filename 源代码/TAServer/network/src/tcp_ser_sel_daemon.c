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
#include<syslog.h>
//#include<errno.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/select.h>
#include<sys/stat.h>
VectorFD *vfd;
int sockfd;

/*
fd对应与某个连接的客户端
和某一个连接的客户端进行双向通信

*/

void do_service(int fd){
	char buff[512];
	memset(buff,0,sizeof(buff));
	size_t size = read(fd,buff,sizeof(buff));
	char *buf = "client closed\n";
//因为采用非阻塞的方式，若读不到数据直接返回
//直接服务下一个客户端
//因此不需要判断size小于0的情况
	if(size==0){//客户端已经关闭连接
//fcntl函数设置的非阻塞方式读写，其读写函数必须用不带缓存的io函数
//而select函数调用，内核会优化io读写，所以可以使用带缓存的io函数和
//不带缓存的函数
		syslog(LOG_DEBUG,"%s\n",buf);
//		write(STDOUT_FILENO,buf,sizeof(buf));
//从动态数组中删除对应的fd，关闭对应客户端的socket
		remove_fd(vfd,fd);
		close(fd);
	}else if(size>0){
		syslog(LOG_DEBUG,"client info:%s\n",buff);
//		write(STDOUT_FILENO,buff,sizeof(buff));
		if(write(fd,buff,size)<0){
			if(errno==EPIPE){//判断对方客户端是否已经关掉，客户端关掉的话则会产生EPIPE信号
				syslog(LOG_DEBUG,"write:%s\n",strerror(errno));
				remove_fd(vfd,fd);
				close(fd);
			}
		}
		
	}
	
}
/*
*遍历出动态数值中的所有描述副并加入到描述副集set中
*同时次函数返回数组中最大的那个描述副
*
*/
int add_set(fd_set *set){
	FD_ZERO(set);
	int max_fd = vfd->fd[0];
	int i=0;
	int fd;
	for(;i<vfd->counter;i++){
		fd = get_fd(vfd,i);
		if(fd>max_fd)
			 max_fd = fd;
		FD_SET(fd,set);
	}
	return max_fd;
}
void * th_fn(void *arg){
	struct timeval t;
	t.tv_sec = 2;
	t.tv_usec=0;
	int n =0;
	int maxfd;
	fd_set set;
	maxfd = add_set(&set);
	int fd;
/*
*调用select函数会阻塞，委托内核去检查传入的描述副是否准备好
*若有则返回准备好的描述副数量，超时返回0；
×第一个参数e为描述副中描述副的范围
*/	
	while((n=select(maxfd+1,&set,NULL,NULL,&t))>=0){
		if(n>0){
	/*
	检查那些描述副准备好
	并和这些准备好的描述副对应的客户端进行数据双向通信
	*/
			int i = 0;
			for(;i<vfd->counter;i++){
				fd = get_fd(vfd,i);
				if(FD_ISSET(fd,&set)){
					do_service(fd);
				}
			}
		}
		//超时重新设置时间和清空描述副集
		t.tv_sec =2;
		t.tv_usec =0;
		//重新遍历动态数组中最新的描述副放置到描述副集中
		maxfd = add_set(&set);
	}
	return (void*)0;
}
void out_addr(struct sockaddr_in *clientaddr){
	char ip[16];
	memset(ip,0,sizeof(ip));
	int port = ntohs(clientaddr->sin_port);
	inet_ntop(AF_INET,
		&clientaddr->sin_addr.s_addr,ip,sizeof(ip));
	syslog(LOG_DEBUG,"%s(%d) connected\n",ip,port);
}
int main(int argc,char *argv[]){
	if(argc<2){
		printf("usage:%s #port\n",argv[0]);
		exit(1);
	}
	//创建守护进程的5个步骤

	//1,创建屏蔽字为0
	umask(0);
	//2调用fork函数创建子进程，父进程退出
	pid_t pid=fork();
	if(pid>0) exit(0);
	//3调用setsid函数创建一个新会话
	setsid();
	//4将当前工作目录更改为根目录
	chdir("/");
	//5关闭不需要的文件描述副
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	//打开系统日志服务的一个连接
	
	openlog(argv[0],LOG_PID,LOG_SYSLOG);
	
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
	//将日志信息写入系统日志文件中
		syslog(LOG_DEBUG,"socket:%s\n",strerror(errno));
		exit(1);
	}
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons((short)atoi(argv[1]));
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	if(bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
		syslog(LOG_DEBUG,"bind:%s\n",strerror(errno));
		exit(1);
	}
	if(listen(sockfd,10)<0){
		syslog(LOG_DEBUG,"listen:%s\n",strerror(errno));
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
		syslog(LOG_DEBUG,"pthread_create:%s\n",strerror(errno));
		exit(1);
	}
	pthread_attr_destroy(&attr);
	/*
	*主控线程获得客户端的连接，将新的socket描述副
	*放置到动态数组中
	*
	*
	*启动的子线程调用select函数委托内核去检查传入到select中
	*的描述副是否准备好
	*利用FD_ISSET找出准备号的那些描述副
	*并和对应的客户端进行双向通信（采用非阻塞方式读写）
	*/
	struct sockaddr_in clientaddr;
	socklen_t clientaddr_len = sizeof(clientaddr);
	while(1){
		//主控線程負責調用accept函數去獲得客戶端的連接
		int fd = accept(sockfd,(struct sockaddr*)&clientaddr,
				&clientaddr_len);
		if(fd<0){
			syslog(LOG_DEBUG,"accept:%s\n",strerror(errno));
			continue;
		}
		out_addr(&clientaddr);
		add_fd(vfd,fd);
	}
	return 0;
}
