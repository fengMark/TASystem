
#include "taregisterctl.h"
#include <model/tatcpsocket.h>

#include <QMessageBox>

// mark: public------------------------------------------------------
TARegisterCtl::TARegisterCtl(QObject *parent) :
    QObject(parent)
{
    m_tcpSocket = new TATcpSocket(this);
    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(requestRegister()));
    connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(resetBtnRegister()));
    connect(m_tcpSocket, SIGNAL(disconnected()),this, SLOT(resetBtnRegister()));
}


TARegisterCtl::~TARegisterCtl()
{

}
void TARegisterCtl::registerAccount(const UserInformation &user)
{
    m_user = user;
    if (m_tcpSocket->isConnected())
    {
        requestRegister();
    }
    else
    {
        m_blockSize = 0;
        m_tcpSocket->requestConnect();
    }
}
void TARegisterCtl::requestRegister()
{
    m_blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    out << quint16(0) << int(REGISTER)  << m_user;
    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));
    m_tcpSocket->write(block);
}
void TARegisterCtl::readMessage()
{
    QDataStream in(m_tcpSocket);
    in.setVersion(QDataStream::Qt_4_7);
    if (m_blockSize == 0)
    {
        if (m_tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> m_blockSize;
    }

    if (m_tcpSocket->bytesAvailable() < m_blockSize)
        return;
    int type;

    in >> type;

    switch (type)
    {
    case REGISTER_SUCCESS:
        in >> m_user.TA_userID >> m_user.TA_regDateTime;
        QMessageBox::about(NULL, tr("注册结果"),
                           tr("%1, 你好！ 恭喜,注册成功. "
                              "您的ID是%2, 请牢牢记住. \n注册时间：%3")
                           .arg(m_user.TA_nickname,
                                m_user.TA_userID,
                                m_user.TA_regDateTime.
                                toString("yyyy-MM-dd hh:mm:ss")));
        break;
    case REGISTER_FAIL:
        QMessageBox::about(NULL, tr("注册结果"), tr("注册失败"));
    }
    emit signalResetBtnRegister(true);

    m_blockSize = 0;
}
void TARegisterCtl::resetBtnRegister()
{
    emit signalResetBtnRegister(true);
}
