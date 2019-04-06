#ifndef TATCPSERVER_H
#define TATCPSERVER_H

#include <QTcpServer>
#include<model/TAConstant.h>
#include<QMap>
#include<control/tadatabasectl.h>
#include<control/taclientsocketctl.h>
#include<model/tathread.h>
class TATcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TATcpServer(QObject *parent = 0);
    ~TATcpServer();

public slots:


private:
    void incomingConnection(int socketDescriptor);
private:
    TAThread *thread;
};

#endif // TATCPSERVER_H
