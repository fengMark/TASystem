#ifndef TASERVERMESSAGECTL_H
#define TASERVERMESSAGECTL_H

#include <QObject>
#include<QListView>
#include<model/tatcpserver.h>
#include<control/tadatabasectl.h>
#include<control/taclientsocketctl.h>
class TAServerMessageCtl : public QObject
{
    Q_OBJECT
public:
    TAServerMessageCtl(QObject *parent = 0);
    ~TAServerMessageCtl();
    void runServer();
    void stopServer();
    static QHostAddress s_hostAddress;
    static int s_hostPort;
private:
    TATcpServer *m_tcpServer;
    TADatabaseCtl m_databaseCtl;
};

#endif // TASERVERMESSAGECTL_H
