#ifndef TATCPSOCKET_H
#define TATCPSOCKET_H

#include <QObject>
#include<QTcpSocket>
class TATcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    TATcpSocket(QObject *parent = 0);
    void requestConnect();
    bool isConnected();
    void setFlag(int);
    int flag();
    static QString getIP();
    static QString getCurrentDateTime();
    static QHostAddress s_hostAddress;
    static int s_hostPort;
signals:
    void showConnectionStatus(const QString &, bool isLogin = false);
private slots:
    void connectionClosed();
    void connectionCreate();
    void dispalyError(QAbstractSocket::SocketError);
private:
    void linkSignalWithSlot();
    bool m_isConnected;
    int m_flag;
};

#endif // TATCPSOCKET_H
