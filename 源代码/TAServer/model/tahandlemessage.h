#ifndef TAHANDLEMESSAGE_H
#define TAHANDLEMESSAGE_H

#include <QObject>
#include<model/TAConstant.h>
#include<QMap>
#include<control/tadatabasectl.h>
#include<control/taclientsocketctl.h>
#include<model/tathread.h>
class TAHandleMessage : public QObject
{
    Q_OBJECT
public:
    explicit TAHandleMessage(QObject *parent = 0);

    // 在线用户列表
    static QMap<QString, TAClientSocketCtl*> m_userMap;
signals:

public slots:
    void clientDisconnected(const QString &id);
    void sendMessage(const SaveTmpInformation &temp);
private:
    void friendRequest(const SaveTmpInformation & save);
    void flockRequest(const SaveTmpInformation & save);
    void talkRequest(const SaveTmpInformation & save);
    void tellFriendsStatusChanged(const QString &id, int status);
    void tellFriendsInformationChanged(const QString & id);
    void tellMembersFlockDroped(const QString & flockID);
    void tellMembersFlockChanged(const FlockInformation &);
    void tellFlockMemberHaveNewMember(const QString &flockID);
    void tellFlockMemberHaveLeavedMember(const QString & flockID);
private:
    // 临时信息
    SaveTmpInformation m_save;
    // 操作数据库
    TADatabaseCtl m_database;
    // 在线用户状态
    QMap<QString, int> m_userStatus;

};

#endif // TAHANDLEMESSAGE_H
