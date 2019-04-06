#include "tatcpsocket.h"
#include <QMessageBox>
#include <QtNetwork>


QHostAddress TATcpSocket::s_hostAddress = QHostAddress::Any;
int TATcpSocket::s_hostPort = 0;
TATcpSocket::TATcpSocket(QObject *parent):
    QTcpSocket(parent)
{
    m_flag = 0;
    m_isConnected = false;
    linkSignalWithSlot();
}
void TATcpSocket::requestConnect()
{
    if (!m_isConnected)
    {
        abort();
        connectToHost(TATcpSocket::s_hostAddress, TATcpSocket::s_hostPort);
        m_isConnected=true;
    }
}
bool TATcpSocket::isConnected()
{
    return m_isConnected;
}
void TATcpSocket::setFlag(int flag)
{
    m_flag = flag;
}
int TATcpSocket::flag()
{
    return m_flag;
}
QString TATcpSocket::getIP()
{
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

/*************************************************
Function Name： getCurrentDateTime()
Description: 获取当前的日期和时间
*************************************************/
QString TATcpSocket::getCurrentDateTime()
{
    QTime time = QTime::currentTime();
    QDate date = QDate::currentDate();

    return QString("%1  %2").arg(date.toString(Qt::ISODate))
        .arg(time.toString(Qt::ISODate));
}
void TATcpSocket::connectionClosed()
{
    m_isConnected = false;
    qDebug("连接断开");
    deleteLater();

    if (m_flag == 0)
    {
        //程序如果和服务器断开连接的话，弹出警告，直接退出
        QMessageBox::critical(NULL, tr("系统错误"),
                              tr("您的客户端已经与服务器断开连接，请重新登录。"));
        qApp->quit();
    }
}
void TATcpSocket::connectionCreate()
{
    m_isConnected = true;
}
void TATcpSocket::dispalyError(QAbstractSocket::SocketError socketError)
{
    m_isConnected = false;
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        emit showConnectionStatus(tr("链接失败.可能是因为服务器关闭."));
        break;
    case QAbstractSocket::HostNotFoundError:
        emit showConnectionStatus(tr("链接失败.可能是因为找不到服务器"));
        QMessageBox::information(NULL, tr("IM Client"),
                                 tr("This host was not found.Please check the"
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        emit showConnectionStatus(tr("链接失败.可能是因为连接被拒绝"));
        QMessageBox::information(NULL, tr("IM Client"),
                                 tr("The connection was refused by the peer."
                                    "Make sure the IM server is running,"
                                    "and check that the host name and port"
                                    "settings are correct."));
        break;
    default:
        emit showConnectionStatus(tr("链接失败: %1.").arg(errorString()));
        QMessageBox::information(NULL, tr("IM Client"),
                                 tr("The following errot occurred: %1.")
                                 .arg(errorString()));
    }
}
void TATcpSocket::linkSignalWithSlot()
{
    connect(this, SIGNAL(connected()), this, SLOT(connectionCreate()));
    connect(this, SIGNAL(disconnected()), this, SLOT(connectionClosed()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(dispalyError(QAbstractSocket::SocketError)));
}
