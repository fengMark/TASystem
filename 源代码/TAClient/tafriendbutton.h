#ifndef TAFRIENDBUTTON_H
#define TAFRIENDBUTTON_H

#include <QPushButton>
#include<model/taconstant.h>
#include<QList>
class TAMainWidget;
class QHBoxLayout;
class TAChatWidget;
class QLabel;
class QMenu;
/*初始化一个好友按钮*/
class TAFriendButton : public QPushButton
{
    Q_OBJECT
public:
    TAFriendButton(const FriendInformation &info, QList<QString> *groupList,TAMainWidget *mainWidget, QWidget *parent = 0);
    ~TAFriendButton();

    bool isRoomOpen() const
    {
        return TA_isOpen;
    }
    bool isRoomShow() const
    {
        return TA_isShow;
    }
    TAChatWidget *getRoomPoint() const
    {
        return TA_chatRoom;
    }
    FriendInformation &getInformation()
    {	return TA_info;	}
    FriendInformation getInformation() const
    {	return TA_info;	}
    void setChangedInformation(const FriendInformation & fri)
    {
        if (TA_info.TA_userID != fri.TA_userID)
            return;
        TA_info.TA_headPortrait = fri.TA_headPortrait;
        TA_info.TA_nickname = fri.TA_nickname;
    }
    // 创建聊天窗口
    void openChatRoom();
    // 显示聊天窗口
    void showChatRoom();
    // 关闭聊天窗口
    void closeChatRoom();

    // 设置button显示信息
    void setButtonStatus();
    // 刷新菜单中分组信息
    void refreshMoveMenu();

signals:
    void deleteFriendSignal(const QString &);
//    void leftDoubleClickSignal();

public slots:
    // 左键 双击
    void onDoubleClickFriendButton();
    // 右键点击按键
    void onClickRightButton(/*const QPoint &*/);

    // 发送即时消息
    void onClickSendMessage();
    // 查看消息记录
//    void onClickShowRecord();
    // 显示好友详细信息
    void onClickShowInformation();
    // 点击修改好友备注
    void onClickChangeRemark();
    // 移动好友至其他分组
    void onClickMove();
    // 删除好友
    void onClickRemoveFriend();

    //聊天窗口关闭了
    void chatRoomQuit();

//    //修改好友状态
//    void changeRemark(const QString &newRemark);
//    void changeStatus(int status);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *);



private:
    // 将好友状态从数字形式转换为字符串形式
    QString getStatusFromInt(const int n) const;
    // 创建右键菜单
    void creatMenu(/*const QList<QString> *groutList*/);


private:
    FriendInformation TA_info;
    //以字符串形式储存好友状态信息
    QString TA_strStatus;
    // 聊天窗口是否打开了
    bool TA_isOpen;
    // 聊天窗口是否显示
    bool TA_isShow;
    // 右键菜单
    QMenu *TA_menu;
    QMenu *TA_moveFriendMenu;
    TAChatWidget *TA_chatRoom;
    TAMainWidget *TA_mainWidget;

    QLabel *TA_labelHead;
    QLabel *TA_labelMes;

    QList<QString> *TA_groupList;

};

#endif // TAFRIENDBUTTON_H
