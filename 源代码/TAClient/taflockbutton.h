#ifndef TAFLOCKBUTTON_H
#define TAFLOCKBUTTON_H

#include <QPushButton>
#include<model/taconstant.h>
class QMenu;
class QLabel;
class TAMainWidget;
class TAFlockWidget;
class TAMainCtl;
class QMouseEvent;
class TAFlockButton : public QPushButton
{
    Q_OBJECT
public:
    TAFlockButton(const FlockInformation &info,TAMainWidget *mainWidget,
                  QWidget *parent = 0);
    ~TAFlockButton();

    bool isRoomOpen() const
    {
        return TA_isOpen;
    }
    bool isRoomShow() const
    {
        return TA_isShow;
    }
    TAFlockWidget *getRoomPoint() const
    {
        return TA_flockRoom;
    }
    FlockInformation &getInformation()
    {	return TA_flockInfo;	}
    FlockInformation getInformation() const
    {	return TA_flockInfo;	}
    void setChangedInformation(const FlockInformation & flock)
    {
        if (TA_flockInfo.TA_flockID != flock.TA_flockID)
            return;

//        TA_flockInfo.TA_creatorID = flock.TA_creatorID;
//        TA_flockInfo.TA_createDate = flock.TA_createDate;
        TA_flockInfo.TA_flockName = flock.TA_flockName;
        TA_flockInfo.TA_theme = flock.TA_theme;
        setButtonStatus();
    }
    // 打开聊天窗口
    void openChatRoom();
    // 显示聊天窗口
    void showChatRoom();
    // 关闭聊天窗口
    void closeChatRoom();

    // 设置button显示信息
    void setButtonStatus();

    // 删除群成员
    void deleteFlockMember(const QString & memberID);
    // 添加群成员
    void addFlockMember(const QString & memberID);

signals:
    void leaveFlockSignal(const QString &);

public slots:
    // 左键 双击
    void onDoubleClickFlockButton();
    // 右键点击按键
    void onClickRightButton();
    // 发送群消息
    void onClickSendMessage();
    // 查看消息记录
    void onClickShowRecord();
    // 显示群详细信息
    void onClickShowInformation();
    // 解散群
    void onClickDropFlock();
    // 退出群
    void onClickLeaveFlock();
    // 添加群成员
    void onClickAddMemberFlock();
    //聊天窗口关闭了
    void chatRoomQuit();

    // 设置成员列表
    void setFlockMemberList(const QVector<FlockMember> &);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    // 创建右键菜单
    void creatMenu(/*const QList<QString> *groutList*/);

private:
    FlockInformation TA_flockInfo;

    // 聊天窗口是否打开了
    bool TA_isOpen;
    // 聊天窗口是否显示
    bool TA_isShow;
    // 右键菜单
    QMenu *TA_menu;
//    TAFlockChatWidget *TA_chatRoom;
    TAMainWidget *TA_mainWidget;
    TAMainCtl *TA_mainCtl;

    QLabel *TA_labelHead;
    QLabel *TA_labelMes;
    TAFlockWidget *TA_flockRoom;
    QVector<FlockMember> TA_flockMembersVec;
};

#endif // TAFLOCKBUTTON_H
