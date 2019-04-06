#include "tatcpserver.h"
#include <model/tathread.h>
#include<QDebug>
TATcpServer::TATcpServer(QObject *parent) :
    QTcpServer(parent)
{
}

TATcpServer::~TATcpServer()
{
}
void TATcpServer::incomingConnection(int socketDescriptor)
{
    thread = new TAThread(0,socketDescriptor);
    thread->start();
}
