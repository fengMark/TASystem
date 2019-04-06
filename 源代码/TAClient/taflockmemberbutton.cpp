#include "taflockmemberbutton.h"
#include<taflockwidget.h>
#include<tamainwidget.h>
#include<QMenu>
#include<QLabel>
#include<control/tamainctl.h>
#include<QHBoxLayout>
#include<QInputDialog>
#include<QMouseEvent>
#include<tachatwidget.h>
TAFlockMemberButton::TAFlockMemberButton(const FlockMember &flockMember,
                                         TAFlockWidget *flockWidget,
                                         TAMainWidget *mainWidget, QWidget *parent) :
    QPushButton(parent)
{
    m_info = flockMember;
    m_flockWidget = flockWidget;
    m_mainWidget = mainWidget;
    m_mainCtrl = m_mainWidget->getMainCtrl();
    m_isOpen = false;

    QHBoxLayout *layout = new QHBoxLayout;

    m_labelHead = new QLabel;
    m_labelHead->setFixedSize(HEAD_MINI_SIZE, HEAD_MINI_SIZE);
    m_labelMes = new QLabel;

    layout->addWidget(m_labelHead);
    layout->addWidget(m_labelMes);
    layout->addStretch();
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    setMinimumHeight(25);

    m_menu = NULL;
    setButtonStatus();
    creatMenu();
    setStyleSheet("QPushButton{border: 0;}");
}

TAFlockMemberButton::~TAFlockMemberButton()
{
}

/*************************************************
Function Name： openChatRoom()
Description: 打开聊天窗口
*************************************************/
void TAFlockMemberButton::openChatRoom()
{
    qDebug() << "chat room with flock member is open";
    if(m_isOpen)
    {
        m_flockWidget->raise();
        m_flockWidget->activateWindow();
        m_flockWidget->show();
        return;
    }
    qDebug() << "create chat room with flock member";
    ChatInformation chatInf;
    chatInf.TA_headPortrait = m_info.TA_headPortrait;
    chatInf.TA_friendID = m_info.TA_userID;
    chatInf.TA_friendNickname = m_info.TA_nickname;
    chatInf.TA_friendRemark = m_info.TA_remark;

    m_isOpen = true;
    TAChatWidget *chatRoom = new TAChatWidget(chatInf);
  //  TA_flockWidget = chatRoom;
    m_mainWidget->insertChatRoomMap(chatInf.TA_friendID, chatRoom);
    connect(chatRoom, SIGNAL(sendMessagesFromChat(TalkMessage &)),m_mainWidget, SLOT(receiveMessagesFromChat(TalkMessage &)));
    connect(chatRoom, SIGNAL(roomQuitSignal()),  this, SLOT(chatRoomQuit()));
    chatRoom->show();
}
void TAFlockMemberButton::closeChatRoom()
{
    if(m_isOpen)
    {
        m_flockWidget->close();
        m_isOpen = false;
    }
}
void TAFlockMemberButton::setButtonStatus()
{
    QString str = QString("resource/image/head/%1.bmp").
        arg(QString::number(m_info.TA_headPortrait));

    QString text, mark;
    m_strFlockStatus = getFlockStatusFromInt(m_info.TA_flockStatus);
    m_strLoginStatus = getLoginStatusFromInt(m_info.TA_loginStatus);
    if(m_info.TA_remark.isEmpty())
        mark = m_info.TA_nickname;
    else
        mark = m_info.TA_remark;
    if(OFFLINE == m_info.TA_loginStatus || INVISIBLE == m_info.TA_loginStatus)
    {
        QPixmap pixmap;
        QIcon icon(str);
        pixmap = icon.pixmap(QSize(HEAD_MINI_SIZE, HEAD_MINI_SIZE),
                             QIcon::Disabled, QIcon::On);
        m_labelHead->setPixmap(pixmap);

        text = QString("<font size=\"FONT_SIEZE\" color=gray>[%1]%2(%3)[%4]</font>").
                arg(m_strFlockStatus, mark, m_info.TA_userID, m_strLoginStatus);
    }
    else
    {
        QPixmap pixmap;
        QIcon icon(str);
        pixmap = icon.pixmap(QSize(HEAD_MINI_SIZE, HEAD_MINI_SIZE));
        m_labelHead->setPixmap(pixmap);

        text = QString("<font size=\"FONT_SIEZE\" color=red>[%1]</font>"
                       "<font size=\"FONT_SIEZE\" color=green>%2</font>"
                       "<font size=\"FONT_SIEZE\" color=blue>(%3)</font>"
                       "<font size=\"FONT_SIEZE\" color=black>[%4]</font>").
                arg(m_strFlockStatus, mark, m_info.TA_userID, m_strLoginStatus);
    }
    m_labelMes->setText(text);

}

//remark:public slots:-----------------------------------------------

// 右键点击按键
void TAFlockMemberButton::onClickRightButton(/*const QPoint &*/)
{
    if (m_menu == NULL)
    {
        creatMenu();
    }
    m_menu->exec(QCursor::pos());
}
void TAFlockMemberButton::onClickSendMessage()
{
    openChatRoom();
}
/*************************************************
Function Name： onClickShowInformation()
Description: 显示成员详细信息
*************************************************/
void TAFlockMemberButton::onClickShowInformation()
{
    m_mainWidget->getFriendInformation(m_info.TA_userID);
}

/*************************************************
Function Name： onClickChangeRemark()
Description: 修改群名片
*************************************************/
void TAFlockMemberButton::onClickChangeRemark()
{
    bool isOk = false;
    QString remark = QInputDialog::getText(NULL, "修改群名片",
                                           "请输入新的群名片",
                                           QLineEdit::Normal,
                                           m_info.TA_remark,
                                           &isOk);

    if (isOk && 0 != remark.compare(m_info.TA_remark))
    {
        TempStrings tmpStr;
        tmpStr.TA_one = m_info.TA_flockID;
        tmpStr.TA_two = m_info.TA_userID;
        tmpStr.TA_three = remark;
        m_mainWidget->changeFlockMemberRemark(tmpStr);
    }

}



/*************************************************
Function Name： onClickRemoveMember
Description: 移除该成员
*************************************************/
void TAFlockMemberButton::onClickRemoveMember()
{
    QMessageBox::StandardButton returnBtn;

    returnBtn = QMessageBox::question(NULL, tr("移除群成员"),
        QString(tr("你确定要将成员%1,从群%2中移除吗？")).
            arg(m_info.TA_userID,m_flockWidget->getFlockInformation().TA_flockID),
        QMessageBox::Yes | QMessageBox::No,  QMessageBox::No);

    if (m_mainWidget == NULL)
        return;
    if(QMessageBox::Yes == returnBtn)
        m_mainWidget->leaveFlock(m_info.TA_userID,
                                 m_flockWidget->getFlockInformation().TA_flockID);
}
void TAFlockMemberButton::onClickAddFriend()
{
    m_mainWidget->addFriend(m_info.TA_userID);
}
void TAFlockMemberButton::onDoubleClickMemberButton()
{
    openChatRoom();
}
void TAFlockMemberButton::chatRoomQuit()
{
    m_isOpen = false;
//    if (NULL != m_flockWidget)
//        m_flockWidget->removeChatWidget(m_info.TA_userID);
}
void TAFlockMemberButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        onClickRightButton();
        return;
    }
    QPushButton::mousePressEvent(event);
}
void TAFlockMemberButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        onDoubleClickMemberButton();
        return;
    }
    QPushButton::mouseDoubleClickEvent(event);
}
QString TAFlockMemberButton::getFlockStatusFromInt(const int n) const
{
    switch (n)
    {
    case FLOCK_HOST:
        return tr("群主");
        break;
    case FLOCK_ADMAIN:
//        return tr("管理员");
//        break;
    case FLOCK_MEMBER:
        return tr("成员");
        break;
    default:
        break;
    }
    return NULL;
}
QString TAFlockMemberButton::getLoginStatusFromInt(const int n) const
{
    switch (n)
    {
    case ONLINE:
        return tr("在线");
        break;
    case OFFLINE:
    case INVISIBLE:
        return tr("离线");
        break;
    case BUSY:
        return tr("忙碌");
        break;
    case LEAVE:
        return tr("离开");
        break;
    case NOTDISTURB:
        return tr("请勿打扰");
        break;
    default:
        break;
    }
    return NULL;
}
void TAFlockMemberButton::creatMenu()
{
    m_menu = new QMenu(this);
    QAction *sendMessage = new QAction(tr("发送即时消息"), m_menu);
    QAction *showInfo = new QAction(tr("查看资料"), m_menu);
    QAction *removeMember= new QAction(tr("移除该成员"), m_menu);
    QAction *changeRemark = new QAction(tr("修改群名片"), m_menu);
    QAction *addRriend= new QAction(tr("加为好友"), m_menu);

    connect(sendMessage, SIGNAL(triggered()),
        this, SLOT(onClickSendMessage()));
    connect(showInfo, SIGNAL(triggered()),
        this, SLOT(onClickShowInformation()));
    connect(removeMember, SIGNAL(triggered()),
        this, SLOT(onClickRemoveMember()));
    connect(changeRemark, SIGNAL(triggered()),
        this, SLOT(onClickChangeRemark()));
    connect(addRriend, SIGNAL(triggered()),
        this, SLOT(onClickAddFriend()));


    // 自己
    if (m_mainWidget->getLocalMyInformation().TA_userID == m_info.TA_userID)
    {
//        m_menu->addAction(showInfo);
        m_menu->addAction(changeRemark);

    }

    // 不是自己 - 是群主
    else if (m_mainWidget->getLocalMyInformation().TA_userID ==
             m_flockWidget->getFlockInformation().TA_creatorID)
    {
        m_menu->addAction(sendMessage);
        m_menu->addAction(showInfo);
        m_menu->addSeparator();
        m_menu->addAction(changeRemark);
        m_menu->addAction(removeMember);

        // 不是好友
        if (!m_mainWidget->isMyFriend(m_info.TA_userID))
        {

            m_menu->addAction(addRriend);
        }
    }
    // 不是自己 - 不是群主
    else
    {
        m_menu->addAction(sendMessage);
        m_menu->addAction(showInfo);
        m_menu->addSeparator();

        // 不是好友
        if (!m_mainWidget->isMyFriend(m_info.TA_userID))
        {
            m_menu->addAction(addRriend);
        }
    }
}
