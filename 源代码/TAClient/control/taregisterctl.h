#ifndef TAREGISTERCTL_H
#define TAREGISTERCTL_H
#include<model/tatcpsocket.h>
#include <QObject>
#include<model/taconstant.h>
class TARegisterCtl : public QObject
{
    Q_OBJECT
public:
    explicit TARegisterCtl(QObject *parent = 0);
    ~TARegisterCtl();
    void registerAccount(const UserInformation &);
signals:
    void signalResetBtnRegister(bool );
private slots:
    void requestRegister();
    // 读取信息
    void readMessage();
    // 重置按钮
    void resetBtnRegister();
private:

private:
    UserInformation m_user;
    quint16 m_blockSize;
    TATcpSocket *m_tcpSocket;
};

#endif // TAREGISTERCTL_H
