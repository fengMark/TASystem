#ifndef _U_MSG_H_
#define _U_MSG_H_

typedef struct{
        struct sockaddr_in sockaddr;
        char buffer[1024];
}Package;

#endif
