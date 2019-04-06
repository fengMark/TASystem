#include<malloc.h>
#include<assert.h>
#include<stdlib.h>
#include<memory.h>
#include"vector_pollfd.h"

static void encapacity(VectorPollFD *vfd){
	if(vfd->counter>=vfd->max_counter){
		struct pollfd *fds=(struct pollfd*)calloc(vfd->counter+5,sizeof(struct pollfd));
		assert(fds!=NULL);
		memcpy(fds,vfd->pfd,sizeof(struct pollfd)*vfd->counter);
		free(vfd->pfd);
		vfd->pfd = fds;
		vfd->max_counter+=5;
	}
}
static int indexof(VectorPollFD *vfd,struct pollfd fd){
	int i = 0;
	for(;i<vfd->counter;i++){
		if(vfd->pfd[i].fd==fd.fd){
			return i;
		}
	}
	return -1;
}
VectorPollFD* create_vector_fd(){
	VectorPollFD *vfd = (VectorPollFD*)calloc(1,sizeof(VectorPollFD));
	assert(vfd!=NULL);
	vfd->pfd=(struct pollfd*)calloc(5,sizeof(struct pollfd));
	assert(vfd->pfd!=NULL);
	vfd->counter = 0;
	vfd->max_counter=5;
	return vfd;
}
void destroy_vector_fd(VectorPollFD *vfd){
	assert(vfd!=NULL);
	free(vfd->pfd);
	free(vfd);
}
struct pollfd* get_fd(VectorPollFD *vfd,int index){
	assert(vfd!=NULL);
	if(index<0||index>vfd->counter-1){
			return NULL;
	}
	return &(vfd->pfd[index]);
}
void remove_fd(VectorPollFD *vfd,struct pollfd fd){
	assert(vfd!=NULL);
	int index = indexof(vfd,fd);
	if(index==-1)	return ;
	int i = index;
	for(;i<vfd->counter-1;i++){
		vfd->pfd[i]=vfd->pfd[i+1];
	}
	vfd->counter--;
}
void add_fd(VectorPollFD *vfd,struct pollfd fd){
	assert(vfd!=NULL);
	encapacity(vfd);
	vfd->pfd[vfd->counter].fd =fd.fd;
	vfd->pfd[vfd->counter].events = fd.events;
	vfd->counter++;
}

