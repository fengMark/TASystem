#include "tareiverudpsockeet.h"

TAReiverUdpSockeet::TAReiverUdpSockeet(QString &filename,QObject *parent) :
    filename(filename),QUdpSocket(parent)
{
    file.setFileName(filename);
    initSocket();
}
TAReiverUdpSockeet::~TAReiverUdpSockeet(){
    file.close();
}
void TAReiverUdpSockeet::initSocket(){
    bind(QHostAddress::Any, 7755);
    connect(this, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));
}
void TAReiverUdpSockeet::readPendingDatagrams(){
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Unbuffered))
        return;
    while (hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);
        file.write(datagram.data(),datagram.size());
        emit byread(datagram.size());
    }
}
