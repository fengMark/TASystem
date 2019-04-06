#include "taservermessagectl.h"
#include <model/tatcpserver.h>
#include <control/taclientsocketctl.h>
#include <QMessageBox>
#include <QDateTime>
#include <QStringListModel>
#include <QListView>
#include <QStringList>
#include<QTcpServer>
QHostAddress TAServerMessageCtl::s_hostAddress = QHostAddress::Any;
int TAServerMessageCtl::s_hostPort = 0;
TAServerMessageCtl::TAServerMessageCtl(QObject *parent) :
    QObject(parent)
{
    m_tcpServer = new TATcpServer(this);
}

TAServerMessageCtl::~TAServerMessageCtl()
{
    if(m_tcpServer!=NULL){
        delete m_tcpServer;
        m_tcpServer=NULL;
    }
}
void TAServerMessageCtl::runServer()
{

   if (!m_tcpServer->listen(s_hostAddress, s_hostPort))
   {
       QMessageBox::critical(NULL, tr("TA Server"),tr("Unable to start the server: %1").arg(m_tcpServer->errorString()));
       exit(EXIT_FAILURE);
   }
   m_databaseCtl.initDatabase();
}
void TAServerMessageCtl::stopServer(){
    m_tcpServer->close();
    m_databaseCtl.close();
    this->~TAServerMessageCtl();
}
