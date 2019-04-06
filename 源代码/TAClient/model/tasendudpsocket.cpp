#include "tasendudpsocket.h"
#include<QDebug>
TASendUdpSocket::TASendUdpSocket(QString friendIP,QString filename,QObject *parent) :
    friendIP(friendIP),QUdpSocket(parent)
{
    initSocket();
    file.setFileName(filename);
}
TASendUdpSocket::~TASendUdpSocket(){
    file.close();
}
void TASendUdpSocket::sendData(){
    int size=0;
    if (!file.open(QIODevice::ReadOnly))
         return;
    while(!file.atEnd()) {
        QByteArray line = file.read(8000);
        qDebug()<<line.size();
        size = line.size();
        emit bytesWritten(size);
        writeDatagram( line , QHostAddress(friendIP),7755);
    }
}
void TASendUdpSocket::initSocket(){
    bind(QHostAddress::LocalHost, 7754);
}
