#ifndef TASENDUDPSOCKET_H
#define TASENDUDPSOCKET_H

#include <QUdpSocket>
#include<QFile>
class TASendUdpSocket : public QUdpSocket
{
    Q_OBJECT
public:
    explicit TASendUdpSocket(QString friendIP,QString filename,QObject *parent = 0);
    ~TASendUdpSocket();
    void sendData();
signals:
private:
    void initSocket();
    QFile file;
    QString friendIP;
};

#endif // TASENDUDPSOCKET_H
