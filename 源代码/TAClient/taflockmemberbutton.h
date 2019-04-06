#ifndef TAFLOCKMEMBERBUTTON_H
#define TAFLOCKMEMBERBUTTON_H

#include <QPushButton>

#include<model/taconstant.h>

class TAMainWidget;
class TAFlockWidget;
class QLabel;
class QMenu;
class TAMainCtl;
class QMouseEvent;

class TAFlockMemberButton : public QPushButton
{
    Q_OBJECT
public:
    TAFlockMemberButton(const FlockMember &, TAFlockWidget *,
                        TAMainWidget *, QWidget *parent = 0);
    ~TAFlockMemberButton();

//    FlockMember &getInformation()
//    {	return m_info;	}
    FlockMember getInformation() const
    {	return m_info;	}
    void setChangedInformation(const FlockMember & mem)
    {
        if (m_info.TA_flockID != mem.TA_flockID)
            return;
        if (m_info.TA_flockID != mem.TA_userID)
            return;

        m_info.TA_headPortrait = mem.TA_headPortrait;
        m_info.TA_nickname = mem.TA_nickname;
        // m_loginStatus;
        m_info.TA_remark = mem.TA_remark;
        // m_flockStatus;
        // m_joinDate



    }

    void setRemark(const TempStrings & tmpStr)
    {
        if (0 != tmpStr.TA_one.compare(m_info.TA_flockID))
            return;
        if (0 != tmpStr.TA_two.compare(m_info.TA_userID))
            return;

        m_info.TA_remark = tmpStr.TA_three;

        setButtonStatus();
    }

    // 打开聊天窗口
    void openChatRoom();
    // 关闭聊天窗口
    void closeChatRoom();
    // 设置button显示信息
    void setButtonStatus();

signals:
    void deleteMemberSignal(const QString &);
public slots:

    // 右键点击按键
    void onClickRightButton(/*const QPoint &*/);

    // 发送即时消息
    void onClickSendMessage();
    // 显示成员详细信息
    void onClickShowInformation();
    // 修改群名片
    void onClickChangeRemark();
    // 移除该成员
    void onClickRemoveMember();
    // 加为好友
    void onClickAddFriend();

    // 左键 双击
    void onDoubleClickMemberButton();
    //聊天窗口关闭了
    void chatRoomQuit();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *);


private:
    // 将好友状态从数字形式转换为字符串形式
    QString getFlockStatusFromInt(const int n) const;

    // 将好友在线状态从数字形式转换为字符串形式
    QString getLoginStatusFromInt(const int n) const;
    // 创建右键菜单
    void creatMenu(/*const QList<QString> *groutList*/);


private:
    FlockMember m_info;
    QString m_strFlockStatus; // 身份
    QString m_strLoginStatus; // 在线状态
    // 聊天窗口是否打开了
    bool m_isOpen;

    // 右键菜单
    QMenu *m_menu;
//    TAChatWidget *m_chatRoom;
    TAFlockWidget *m_flockWidget;
    TAMainWidget *m_mainWidget;
    TAMainCtl *m_mainCtrl;

    QLabel *m_labelHead;
    QLabel *m_labelMes;
};

#endif // TAFLOCKMEMBERBUTTON_H
