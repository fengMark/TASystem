#ifndef TAMAINWIDGET_H
#define TAMAINWIDGET_H

#include<model/taconstant.h>
#include<control/tadatabasectl.h>
#include<QStringList>
#include<QWidget>
class QToolButton;
class QListWidget;
class QMessageBox;
class QInputDialog;
class QLineEdit;
class QCloseEvent;
class QMouseEvent;
class QTimerEvent;
class QLabel;
class QMenu;
class TAEncryption;
class TADatabaseCtl;
class TAFlockInformationWidget;
class TAInformationWidget;
class TAClickLabel;
class TAFriendButton;
class TAFlockButton;
class TAToolItem;
class TAMainCtl;
class TAChatWidget;
class TAFlockWidget;
class TALinkFriendButton;
class TALinkFlockButton;
class TALatestMessageListWidget;
class TAFlockListWidget;
class TASearchListWidget;
class TAFriendListWidget;
class TAFlockInformationWidget;
class TALinkButton;
namespace Ui {
    class TAMainWidget;
}

class TAMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TAMainWidget( const UserInformation me, QWidget *parent = 0);
    ~TAMainWidget();
    TAMainCtl * getMainCtrl() const
    {
        return TAmainCtl;
    }
    // 修改群信息（发送至服务器）
    void changeFlockInformation(const FlockInformation &);
    // 修改群成员名片（发送至服务器）
    void changeFlockMemberRemark(const TempStrings & tmpStr);
    // 添加好友（发送至服务器）
    void addFriend(const QString friendID);
    bool isMyFriend(const QString &id) const;
    void uploadIconFile(const QString &iconname);
    // 删除存储与m_flockInformationMap中的信息窗口
    void removeFlockInformationWidget(const QString &);
    // 删除存储于m_flockRoomMap中的群聊窗口
    void removeFlockChatWidget(const QString &);
    // 退出群（发送至服务器）
    void leaveFlock(const QString & memberID, const QString &flockID);
    UserInformation getLocalMyInformation() const;
    void dropFlock(const QString &);
    void setOnlineCounter(int num);
    void getFriendsInformation();
    void getFlocksInformation();
    void getLatestMessage();
    void getFriendInformation(const QString &id);
    void insertChatRoomMap(const QString &, TAChatWidget *);
    void insertFlockRoomMap(const QString &, TAFlockWidget *);
    void upIconFile(QString *);//上传头像文件到服务器
    void removeInformationWidget(const QString &);
    void deleteFriend(const QString &);
    void moveFriendToBox(const QString & friendID, const QString & groupName, const QString &title);
    void removeChatWidget(const QString &);
    void changeMyInformation(const UserInformation &me);
    void changeFriendRemark(const QString &, const QString &);
protected:
    void closeEvent(QCloseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void timerEvent(QTimerEvent *);

public slots:
    void showLatestMessageWidget();
    void setTimerStatus(bool isOpen);
    void leavedFlock(const QString &, const QString &, const QString &);
    void setFlockMemberRemark(const TempStrings &);
    void createFlockSuccess(const FlockInformation & flock);
    void moveFriendToBoxSuccess(const QString &, const QString &, const QString &);
    void createBox();
    void receiveMessagesFromChat(TalkMessage &mes);
    void receiveFlockRequest(const TalkMessage &,const FlockMember &,const FlockInformation &);
    void setFriendStatus(const QString &, int);
    void changeMyStatus(const int index);
    void renameBoxSuccess(const QString & title, const QString & newTitle);
    void renameBox(const QString & title);
    void removeBox(const QString & title);
    void receiveFriendRequest(const TalkMessage & mes,const UserInformation & friendInf);
    void showFlockInformation(const FlockInformation &);
    void closeAllWindows();
    void setMyInformation(const UserInformation &user);
    void setFriendListWidget(const QVector<FriendInformation> &);
    void setFlockListWidget(const QVector<FlockInformation> & flocksVec);
    void setFlockInformation(const FlockInformation &);
    void setFriendRemark(const TempStrings&);
    void showFriendInformation(const UserInformation &);
    void showMyInformation(const UserInformation &me);
    bool addFriendButton(const FriendInformation &);
    bool addFlockButton(const FlockInformation &);
    void searchFriend(const QString & text);
    void removeLinkButton(const QString & id);
    void onClickSearchFriend();
    void onClickChangePwd();
    void onClickRightButton();
    void onClickAddFriend();
    void onClickAddFlock();
    void createFlock();
    void receiveFriendTalkMessage(const TalkMessage &);
    void deletedFriend(const QString &, const QString &, const QString &);
    void getMyInformation();
    void closeWindow();
private:
    void initTAMainWidget();
    void linkSignalWithSlot();
    void clearContainers();
    void creatMenu();
    void refreshFriendButtonMenu();
    bool continueToClose();
private:
    Ui::TAMainWidget *ui;
    QListWidget *searchList;
    TAMainCtl *TAmainCtl;
    int TAonlineCounter;
    UserInformation TAmyself;
    QLabel *nicklabel;
    QMenu *TAmenu;
    QToolButton *TAtoolBtnsearch;
    QLineEdit *TAsearchline;
    TAClickLabel *headlabel;
    QMap<QString,TAChatWidget *> TAchatRoomMap; // 好友ID 与 聊天室 映射表
    QMap<QString,TAFlockWidget *> TAflockRoomMap; // 群ID 与 群聊 映射表

    QMap<QString, TAInformationWidget *> TAinformationMap;//用户信息显示
    QMap<QString, TAFlockInformationWidget *> TAflockInformationMap;//群信息显示

    QStringList TAfriendsGroupList;  // 分组名 列表
    QStringList TAfriendsMarkList; // 某分组中好友 备注名 列表
    QStringList TAfriendsIDList; // 某分组中好友 ID  列表

    QVector<TAToolItem *> TAtoolItemsFriendsVec;  // 存储每个分组的向量

    QMap<QString, int> TAindexFriendsGroupMap; // 组名 与 下标 映射表

    QMap<QString, TAFriendButton *> TAfriendMap; // 好友帐号 与 好友按钮 映射表

    QMap<QString, TAFlockButton *> TAflockMap; // 群ID 与 群按钮 映射表

    QMap<QString, TALinkButton*> TAlinkMap; //ID 链接按钮 映射表

    TAFriendListWidget *TAfriendlistWidget;
    TAFlockListWidget *TAflocklistWidget;
    TALatestMessageListWidget *TAmessagelistWidget;
    TASearchListWidget *TAsearchlistWidget;
    int TAtimerId; //ID of timer
    int TAflag;
    // 操作数据库
    TADatabaseCtl TAdatabase;
    QVector<TalkMessage> TAlocalHistoryMessageVec;
    QVector<TalkMessage> TAnetworkHistoryMessageVec;
};

#endif // TAMAINWIDGET_H
