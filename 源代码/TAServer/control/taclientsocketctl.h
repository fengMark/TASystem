#ifndef TACLIENTSOCKETCTL_H
#define TACLIENTSOCKETCTL_H

#include <QObject>
#include<QTcpSocket>
#include<model/tauser.h>
#include<model/TAConstant.h>
#include<control/tadatabasectl.h>
class TAClientSocketCtl : public QTcpSocket
{
    Q_OBJECT
public:
    TAClientSocketCtl(QObject *parent = 0);
    ~TAClientSocketCtl();
signals:
    void sendSignal(const SaveTmpInformation &temp);
    void userLoginSignal(const UserInformation &user);
    void deleteSignal(const QString &acc);
public slots:
    void receiveMessage();
    void sendMessage(const SaveTmpInformation &temp);
    void deleteScoket();

private:
    quint16 m_blockSize;
    SaveTmpInformation m_save;

private:
    TADatabaseCtl m_database;

};

#endif // TACLIENTSOCKETCTL_H
