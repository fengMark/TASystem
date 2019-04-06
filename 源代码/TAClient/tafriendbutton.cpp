#include "tafriendbutton.h"
#include<tachatwidget.h>
#include<tamainwidget.h>
#include<QLabel>
#include<QMenu>
#include<QHBoxLayout>
#include<QInputDialog>
#include<QLineEdit>
#include<QDebug>
TAFriendButton::TAFriendButton(const FriendInformation &info,
                               QList<QString> *groupList,
                               TAMainWidget *mainWidget,
                               QWidget *parent) :
    QPushButton(parent)
{
    TA_info = info;
    TA_mainWidget = mainWidget;
    TA_isOpen = false;
    TA_isShow = false;

    QHBoxLayout *layout = new QHBoxLayout;
    TA_labelHead = new QLabel;
    TA_labelHead->setFixedSize(HEAD_SMALL_SIZE, HEAD_SMALL_SIZE);
    TA_labelMes = new QLabel;
    layout->addWidget(TA_labelHead);
    layout->addWidget(TA_labelMes);
    layout->addStretch();
    layout->setContentsMargins(0,0,0,0);

    setLayout(layout);
    setMinimumSize(100,40);

    TA_menu = NULL;
    TA_moveFriendMenu = NULL;
    TA_groupList = groupList;
    setButtonStatus();

    setStyleSheet("QPushButton{border: 0;}");
}


TAFriendButton::~TAFriendButton()
{

}
void TAFriendButton::openChatRoom()
{
    if(TA_isOpen)
    {
        qDebug()<<"has open";
        return;
    }

    ChatInformation chatInf;
    chatInf.TA_myID = TA_mainWidget->getLocalMyInformation().TA_userID;
    chatInf.TA_headPortrait = TA_info.TA_headPortrait;
    chatInf.TA_friendID = TA_info.TA_userID;
    chatInf.TA_friendNickname = TA_info.TA_nickname;
    chatInf.TA_friendRemark = TA_info.TA_remarkName;
    chatInf.TA_friendStatus = TA_info.TA_status;

    TA_isOpen = true;
    TAChatWidget *chatRoom = new TAChatWidget(chatInf);

    TA_chatRoom = chatRoom;
    TA_mainWidget->insertChatRoomMap(chatInf.TA_friendID,TA_chatRoom);//绑定聊天室和ID
    connect(TA_chatRoom, SIGNAL(sendMessagesFromChat(TalkMessage &)),TA_mainWidget, SLOT(receiveMessagesFromChat(TalkMessage &)));
    connect(TA_chatRoom, SIGNAL(roomQuitSignal()),  this, SLOT(chatRoomQuit()));
    qDebug()<<"has not open";
}

void TAFriendButton::showChatRoom()
{
    TA_chatRoom->raise();
    TA_chatRoom->activateWindow();
    TA_chatRoom->show();
    TA_isShow = true;
}

/*************************************************
Function Name： closeChatRoom()
Description: 关闭聊天窗口
*************************************************/
void TAFriendButton::closeChatRoom()
{
    if(TA_isOpen)
    {
        TA_chatRoom->close();
        TA_isOpen = false;
        TA_isShow = false;
    }
}
void TAFriendButton::setButtonStatus()
{
    QString str = QString(":/image/resource/image/head/%1.bmp").arg(QString::number(TA_info.TA_headPortrait));
    QString text, mark;
    TA_strStatus = getStatusFromInt(TA_info.TA_status);
    if(TA_info.TA_remarkName.isEmpty())//若备注为空，备注昵称
        mark = TA_info.TA_nickname;
    else
        mark = TA_info.TA_remarkName;
    if(OFFLINE == TA_info.TA_status || INVISIBLE == TA_info.TA_status)
    {
        QPixmap pixmap;
        QIcon icon(str);
        pixmap = icon.pixmap(QSize(HEAD_SMALL_SIZE, HEAD_SMALL_SIZE),
                             QIcon::Disabled, QIcon::On);
        TA_labelHead->setPixmap(pixmap);

        text = QString("<font color=gray>%1(%2)<br>[%3]</font>").
                arg(TA_info.TA_userID).arg(mark).arg(TA_strStatus);
    }
    else
    {
        TA_labelHead->setPixmap(QPixmap(str));

        text = QString("<font color=green>%1(%2)<br>[%3]</font>").
                arg(TA_info.TA_userID).arg(mark).arg(TA_strStatus);
    }
    TA_labelMes->setText(text);
}


/*************************************************
Function Name： refreshMoveMenu()
Description: 刷新菜单中分组信息
*************************************************/
void TAFriendButton::refreshMoveMenu()
{
    if (TA_menu == NULL || TA_moveFriendMenu == NULL)
    {
        return;
    }

    TA_moveFriendMenu->clear();
    QString groupName;
    for (int i = 0; i<TA_groupList->size(); ++i)
    {
        groupName = TA_groupList->value(i);
        if (0 != groupName.compare(TA_info.TA_groupName))
        {
            QAction * action = new QAction(groupName, TA_moveFriendMenu);
            TA_moveFriendMenu->addAction(action);
            connect(action, SIGNAL(triggered()),this, SLOT(onClickMove()));
        }
    }
}

void TAFriendButton::onDoubleClickFriendButton()
{
    openChatRoom();
    showChatRoom();
    TA_mainWidget->removeLinkButton(TA_info.TA_userID);
}

void TAFriendButton::onClickRightButton(/*const QPoint &*/)
{
    if (TA_menu == NULL)
    {
        creatMenu();
    }
    TA_menu->exec(QCursor::pos());
}

void TAFriendButton::onClickSendMessage()
{
    openChatRoom();
}

void TAFriendButton::onClickShowInformation()
{
    TA_mainWidget->getFriendInformation(TA_info.TA_userID);
}

void TAFriendButton::onClickChangeRemark()
{
    bool isOk = false;
    QString remark = QInputDialog::getText(NULL, "修改好友备注","请输入新的备注名称",QLineEdit::Normal,TA_info.TA_remarkName,&isOk);

    if (isOk && 0 != remark.compare(TA_info.TA_remarkName))
    {
        qDebug("request-change-remark");
        TA_mainWidget->changeFriendRemark(TA_info.TA_userID, remark);
    }

}

void TAFriendButton::onClickMove()
{
    QAction* action = dynamic_cast<QAction*>(sender());
    TA_mainWidget->moveFriendToBox(TA_info.TA_userID, TA_info.TA_groupName, action->text());
}

void TAFriendButton::onClickRemoveFriend()
{
    QMessageBox::StandardButton returnBtn;

    returnBtn = QMessageBox::question(NULL, tr("删除好友"),QString(tr("你确定要删除好友(%1)吗？")).arg(TA_info.TA_userID),QMessageBox::Yes | QMessageBox::No,  QMessageBox::No);

    if(QMessageBox::Yes == returnBtn)
        TA_mainWidget->deleteFriend(TA_info.TA_userID);
}

void TAFriendButton::chatRoomQuit()
{
    TA_isOpen = false;
    TA_isShow = false;
    if (NULL != TA_mainWidget)
        TA_mainWidget->removeChatWidget(TA_info.TA_userID);
}

void TAFriendButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        onClickRightButton();
        return;
    }
    QPushButton::mousePressEvent(event);
}

void TAFriendButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        onDoubleClickFriendButton();
        return;
    }
    QPushButton::mouseDoubleClickEvent(event);
}

/*************************************************
Function Name： getStatusFromInt()
Description: 将好友状态从数字形式转换为字符串形式
*************************************************/
QString TAFriendButton::getStatusFromInt(const int n) const
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
void TAFriendButton::creatMenu()
{
    TA_menu = new QMenu(this);
    QAction *sendMessage = new QAction(tr("发送即时消息"), TA_menu);
    QAction *showInfo = new QAction(tr("查看资料"), TA_menu);
    QAction *changeRemar = new QAction(tr("修改备注名称"), TA_menu);
    QAction *removeFriend = new QAction(tr("删除好友"), TA_menu);
    TA_moveFriendMenu= new QMenu(tr("移动好友至"), TA_menu);
    refreshMoveMenu();

    connect(sendMessage, SIGNAL(triggered()),
        this, SLOT(onDoubleClickFriendButton()));

    connect(showInfo, SIGNAL(triggered()),
        this, SLOT(onClickShowInformation()));
    connect(changeRemar, SIGNAL(triggered()),
        this, SLOT(onClickChangeRemark()));
    connect(removeFriend, SIGNAL(triggered()),
        this, SLOT(onClickRemoveFriend()));

    TA_menu->addAction(sendMessage);
    TA_menu->addAction(showInfo);
    TA_menu->addAction(changeRemar);
    TA_menu->addSeparator();

    TA_menu->addMenu(TA_moveFriendMenu);
    TA_menu->addAction(removeFriend);
}
