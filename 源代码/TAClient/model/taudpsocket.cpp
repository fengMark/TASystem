#include "taudpsocket.h"
#include<QTimer>
#include<control/tamainctl.h>
#include<QList>
#include<QHostAddress>
#include<QNetworkInterface>
#include<QAbstractSocket>

int TAUdpSocket::port = 0;
TAUdpSocket::TAUdpSocket(QObject *parent) :
    QUdpSocket(parent)
{
    bind (TAUdpSocket::port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}
QString TAUdpSocket::getIP(){
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol) //我们使用IPv4地址
        {
           if(address.toString()!="127.0.0.1")
                return address.toString();
        }
    }
    return 0;
}
QString TAUdpSocket::getCurrentDateTime(){
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();
    return QString("%1  %2").arg(date.toString(Qt::ISODate))
        .arg(time.toString(Qt::ISODate));
}



