#ifndef TAMAINCTL_H
#define TAMAINCTL_H

#include <QObject>
#include<QVector>
#include<QDate>
#include<QMessageBox>
#include<model/tatcpsocket.h>
#include<model/taconstant.h>
#include<taloginwidget.h>
#include<QTimer>
class TAMainCtl : public QObject
{
    Q_OBJECT
public:
   explicit TAMainCtl(const QString myID, QObject *parent = 0);
    void getFlockMembers(const QString &);
    void sendTalkMessage(TalkMessage &);
    void getFriendsInformation(const QString &, const int);
    void getFriendInformation(const QString &id);
    void getFlocksInformation(const QString &);
    void deleteFriend(const QString & myID, const QString & friendID);
    void moveFriendToBox(const QString &, const QString &, const QString &);
    void changeFriendRemark(const QString & myID,
                            const QString & friendID,
                            const QString &remark);

    // 修改群信息
    void changeFlockInformation(const FlockInformation &);
    // 发送处理群请求结果的请求
    void requestSendResultOfFlockRequest(const TalkMessage & mes);
    // 群请求结结果
    void resultOfFlockRequest(const TalkMessage & mes);
    void resultOfFriendRequest(const TalkMessage & mes);
    // 修改群成员名片
    void changeFlockMemberRemark(const TempStrings & tmpStr);
    void uploadIconFile(const QString &icon_name,const QString &userID);
    // 退出群
    void leaveFlock(const QString &, const QString &);
    // 解散群
    void dropFlock(const QString &, const QString &);
    // 获取自己详细信息
    void getMyinformation(const QString &id);
     void changeMyInformation(const UserInformation &me);
     void getLatestMessage(const QString & id);
     // 添加好友
     void addFriend(const TalkMessage & mes);
     // 加入群
     void addFlock(const TalkMessage & mes);
     // 创建群
     void createFlock(const TempStrings & tmpStr);
     // 修改自己的状态
     void changeMyStatus(const QString id, const int status);
     // 发送修改自己状态的请求
     void requestChangeMyStatus(const QString id, const int status);
     void renameBox(const QString & oldTitle, const QString & newTitle);
     void closeConnect();
public slots:

     // 发送修改群信息的请求
     void requestChangeFlockInformation(const FlockInformation &);
     void requestgetLatestMessage(const QString & id);
     void requestRenameBox(const QString & oldTitle, const QString & newTitle);
     void requestSendResultOfFriendRequest(const TalkMessage & mes);
     // 发送修改群成员名片的请求
     void requestChangeFlockMemberRemark(const TempStrings & tmpStr);
     void requestuploadIconFile(const QString &icon_name,const QString &userID);
     // 发送获取群所有成员的请求
     void requestGetFlockMembers(const QString &);
     // 发送退出群的请求
     void requestLeaveFlock(const QString &, const QString &);
     // 发送解散群的请求
     void requestDropFlock(const QString &, const QString &);
     // 发送获取自己详细信息的请求
     void requestGetMyInformation();
     // 发送移动好友至其他分组的请求
     void requestMoveFriendToBox(const QString & friendID, const QString & oldTitle,
                                 const QString & newTitle);
     // 发送添加好友的请求
    void requestAddFriend(const TalkMessage & mes);
    // 发送加入群的请求
    void requestAddFlock(const TalkMessage & mes);
    void requestChangeMyInformation(const UserInformation &);
    void requestGetFriendsInformation();
    void requestGetFlocksInformation(const QString &);
    void requestDeleteFriend(const QString & myID, const QString & friendID);
    // 发送获取好友详细信息的请求
    void requestGetFriendInformation(const QString &);
    // 发送修改密码的请求
    void requestChangePwd(const QString & oldPwd, const QString & newPwd);
    // 发送修改好友备注的请求
    void requestChangeFriendRemark(const QString & myID,
                                   const QString & friendID,
                                   const QString &remark);
    void requestSendTalkMessage();
    void requestCreateFlock(const TempStrings &);

     void readMessage();

signals:
    void uploadIconFileSuccess(const int &headPortrait);
    void getFriendsSuccess(const QVector<FriendInformation> &);
    void getFlocksSuccess(const QVector<FlockInformation> &);
    void getFlockMembersSuccess(const QVector<FlockMember> &);

    void getFriendTalkMessage(const TalkMessage &);
    void getFriendChangedStatus(const QString &, const int);
    void getFriendChangedInformation(const FriendInformation &);
    void getFriendInformationSuccess(const UserInformation &);
    void getMyInformationSuccess(const UserInformation &);
    void changeMyInformationSuccess(const UserInformation &);
    void changeFriendRemarkSuccess(const TempStrings &);
    void deleteFriendSuccess(const QString &, const QString &, const QString &);
    void getFriendRequest(const TalkMessage &, const UserInformation &);

    void getFlockRequest(const TalkMessage &, const FlockMember &,
                         const FlockInformation &);
    void dropFlockSuccess(const QString &);
    void leaveFlockSuccess(const QString &, const QString &, const QString &);
    void haveMemberleavedFlock(const QString &, const QString &, const QString &);
    void changeFlockSuccess(const FlockInformation &);
    void changeFlockRemarkSuccess(const TempStrings &);
    void createFlockSuccess(const FlockInformation &);

    void renameBoxSuccess(const QString &, const QString &);
    void moveFriendToBoxSuccess(const QString &, const QString &, const QString);

    void getNetworkHistoryMessagesSuccess(const QString & friendID, const QDate & date,
                                   const QVector<TalkMessage> &);
    void closeWindowSignal();
private:
    QString TA_myID;
    TATcpSocket *TA_tcpSocket;
    quint16 TA_blockSize;
    QString TA_id;
    int TA_status;
    int TA_peerStatus;
    int headPortrait;
    QVector<TalkMessage> TA_messageVector;

    QVector<TalkMessage> TA_HistoryMessageVector;
    QVector<TalkMessage> TA_OfflineMessageVector;

    QString TA_creatorID;
    QString TA_flockID;
    QString TA_discussionID;
    FlockMember TA_flockMember;
    FlockInformation TA_flockInf;

    QVector<FriendInformation> TA_friendsVec;
    QVector<FlockInformation> TA_flocksVec;
    QVector<FlockMember> TA_flockMembersVec;

    TalkMessage TA_message;
    QString TA_peerID;
    QString TA_groupName;
    QString TA_friendID;
    UserInformation TA_myInf;
    UserInformation TA_friInf;
    FriendInformation TA_friSimInf;
    TempStrings TA_tempStr;
    QString TA_title;
    QString TA_newTitle;
    QDate TA_date;

    QTimer *timer;
};

#endif // TAMAINCTL_H
