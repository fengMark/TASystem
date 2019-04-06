#include "tathread.h"
#include<model/tahandlemessage.h>
TAThread::TAThread(QObject *parent) :
    QThread(parent)
{
}


TAThread::TAThread( QObject * parent, int socketDescriptor):
    QThread(parent)

{
    m_socketDescriptor = socketDescriptor;
}

TAThread::~TAThread()
{

}
void TAThread::run()
{
    m_clientSocket = new TAClientSocketCtl();
    m_handleMes = new TAHandleMessage();
    m_clientSocket->setSocketDescriptor(m_socketDescriptor);
    connect(m_clientSocket, SIGNAL(deleteSignal(const QString &)),m_handleMes, SLOT(clientDisconnected(const QString &)));
    connect(m_clientSocket, SIGNAL(sendSignal(const SaveTmpInformation &)),m_handleMes, SLOT(sendMessage(const SaveTmpInformation &)));
    exec();
}
