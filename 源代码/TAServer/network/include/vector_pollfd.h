#ifndef __VECTOR_POLL_H_
#define __VECTOR_POLL_H_
#include<poll.h>
typedef struct{
	 struct pollfd *pfd;
	int counter;
	int max_counter;
}VectorPollFD;
extern VectorPollFD* create_vector_fd();
extern void destroy_vector_fd(VectorPollFD *);
extern struct pollfd* get_fd(VectorPollFD *,int index);
extern void remove_fd(VectorPollFD *,struct pollfd fd);
extern void add_fd(VectorPollFD *,struct pollfd fd);
#endif
