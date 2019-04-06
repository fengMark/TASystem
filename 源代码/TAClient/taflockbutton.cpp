#include "taflockbutton.h"
#include<tamainwidget.h>
#include<control/tamainctl.h>
#include<taflockwidget.h>
#include<QMenu>
#include<QLabel>
#include<QInputDialog>
#include<QAction>
#include<QHBoxLayout>
#include<QMouseEvent>
TAFlockButton::TAFlockButton(const FlockInformation &info,
                             TAMainWidget *mainWidget,
                             QWidget *parent) :
    QPushButton(parent)
{
    TA_flockInfo = info;
    TA_mainWidget = mainWidget;
    if (TA_mainWidget == NULL)
        return;
    TA_mainCtl = TA_mainWidget->getMainCtrl();
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
    setButtonStatus();
    creatMenu();
    setStyleSheet("QPushButton{border: 0;}");
}

TAFlockButton:: ~TAFlockButton()
{
}
void TAFlockButton::openChatRoom()
{
    if (TA_mainWidget == NULL)
        return;

    if(TA_isOpen)
    {
        return;
    }
    TA_isOpen = true;

    TAFlockWidget *chatRoom = new TAFlockWidget(TA_flockInfo, TA_mainWidget);
    TA_flockRoom = chatRoom;
    TA_mainWidget->insertFlockRoomMap(TA_flockInfo.TA_flockID, chatRoom);
    connect(chatRoom, SIGNAL(sendMessagesFromChat(TalkMessage &)),
            TA_mainWidget, SLOT(receiveMessagesFromChat(TalkMessage &)));
    connect(chatRoom, SIGNAL(roomQuitSignal()),  this, SLOT(chatRoomQuit()));

    TA_mainCtl->getFlockMembers(TA_flockInfo.TA_flockID);
    connect(TA_mainCtl, SIGNAL(getFlockMembersSuccess(QVector<FlockMember>)),this, SLOT(setFlockMemberList(QVector<FlockMember>)));
    connect(TA_mainCtl, SIGNAL(getFlockMembersSuccess(QVector<FlockMember>)),chatRoom, SLOT(setFlockMemberList(QVector<FlockMember>)));
}
void TAFlockButton::showChatRoom()
{
    TA_flockRoom->raise();
    TA_flockRoom->activateWindow();
    TA_flockRoom->show();
    TA_isShow = true;
}
void TAFlockButton::closeChatRoom()
{
    if(TA_isOpen)
    {
        TA_flockRoom->close();
        TA_isOpen = false;
        TA_isShow = false;
    }
}
void TAFlockButton::setButtonStatus()
{
    QString str = QString(":/image/resource/image/flock.png");
    QPixmap pixmap;
    QIcon icon(str);
    pixmap = icon.pixmap(QSize(HEAD_SMALL_SIZE, HEAD_SMALL_SIZE));
    TA_labelHead->setPixmap(pixmap);
    QString text = QString("%1\n%3").arg(TA_flockInfo.TA_flockName, TA_flockInfo.TA_theme);
    TA_labelMes->setText(text);
}
void TAFlockButton::deleteFlockMember(const QString & memberID)
{
    for (int i =0; i<TA_flockMembersVec.size(); ++i)
    {
        if (0 == memberID.compare(TA_flockMembersVec[i].TA_userID))
        {
            TA_flockMembersVec[i].TA_userID = "0";
        }
    }
}

void TAFlockButton::addFlockMember(const QString & memberID)
{
    int size = TA_flockMembersVec.size();
    TA_flockMembersVec.resize(size + 1);

    TA_flockMembersVec[size].TA_userID = memberID;

}
void TAFlockButton::onDoubleClickFlockButton()
{
    openChatRoom();
    showChatRoom();
    TA_mainWidget->removeLinkButton(TA_flockInfo.TA_flockID);
}
void TAFlockButton::onClickRightButton()
{
    if (TA_menu == NULL)
    {
        creatMenu();
    }
    TA_menu->exec(QCursor::pos());
}
void TAFlockButton::onClickSendMessage()
{
    openChatRoom();
}
void TAFlockButton::onClickShowRecord()
{
}

void TAFlockButton::onClickShowInformation()
{
    if (TA_mainWidget == NULL)
        return;
    TA_mainWidget->showFlockInformation(TA_flockInfo);
}
void TAFlockButton::onClickDropFlock()
{

    QMessageBox::StandardButton returnBtn;

    returnBtn = QMessageBox::question(NULL, tr("解散群"),
        QString(tr("你确定要解散群(%1)吗？")).arg(TA_flockInfo.TA_flockName),
        QMessageBox::Yes | QMessageBox::No,  QMessageBox::No);

    if(QMessageBox::No == returnBtn)
        return;
    if (TA_mainWidget == NULL)
        return;
    TA_mainWidget->dropFlock(TA_flockInfo.TA_flockID);
}
void TAFlockButton::onClickLeaveFlock()
{
    QMessageBox::StandardButton returnBtn;

    returnBtn = QMessageBox::question(NULL, tr("退出群"),
        QString(tr("你确定要退出群(%1)吗？")).arg(TA_flockInfo.TA_flockName),
        QMessageBox::Yes | QMessageBox::No,  QMessageBox::No);

    if (TA_mainWidget == NULL)
        return;
    if(QMessageBox::Yes == returnBtn)
        TA_mainWidget->leaveFlock(TA_mainWidget->getLocalMyInformation().TA_userID,
                                 TA_flockInfo.TA_flockID);
}
void TAFlockButton::onClickAddMemberFlock()
{
    bool isOk = false;
    QString memberID = QInputDialog::getText(NULL, "添加群成员",
                                           "请输入对方帐号",
                                           QLineEdit::Normal,
                                           "",
                                           &isOk);
    if (!isOk)
        return;

    // mark
    // 检验 是否全是 数字 或者 一开始就限制 或者 重写一个messagebox

    if (0 == memberID.compare(TA_mainWidget->getLocalMyInformation().TA_userID))
    {
        QMessageBox::critical(NULL, tr("添加失败"), tr("不能添加自己"));
        return;
    }
    for (int i =0; i<TA_flockMembersVec.size(); ++i)
    {
        if (0 == memberID.compare(TA_flockMembersVec[i].TA_userID))
        {
            QMessageBox::critical(NULL, tr("添加失败"), tr("该用户已经是该群成员"));
            return;
        }
    }

    bool isOkMes = false;
    QString checkMes = QInputDialog::getText(NULL, "添加群成员",
                       "请输入验证信息",
                       QLineEdit::Normal,
                       QString(tr("您好,我是群%1(%2)的群主，请求您加入我们的群")).
                                             arg(TA_flockInfo.TA_flockID,
                                                 TA_flockInfo.TA_flockName),
                       &isOkMes);
    if (!isOkMes)
        return;

    TalkMessage mes;
    mes.TA_senderID = TA_flockInfo.TA_flockID; // 群ID
    mes.TA_receiverID = memberID;  // 成员ID
    mes.TA_text = checkMes;
    mes.TA_type = FLOCK_REQUEST;

    if (NULL != TA_mainCtl)
        TA_mainCtl->addFlock(mes);
}
void TAFlockButton::chatRoomQuit()
{
    TA_isOpen = false;
    TA_isShow = false;
    if (NULL != TA_mainWidget)
        TA_mainWidget->removeFlockChatWidget(TA_flockInfo.TA_flockID);
}
void TAFlockButton::setFlockMemberList(const QVector<FlockMember> & memberList)
{
    int len = memberList.size();
    if (len <= 0) return;


    // 别的flock请求服务器获取群成员的时候，
    // 会影响到别的flock， 可以 判断一下flockID.
    if (0 != memberList[0].TA_flockID.compare(TA_flockInfo.TA_flockID))
        return;

//    getRoomPoint()->setFlockMemberList(memberList);
//    int len = memberList.size();
    TA_flockMembersVec.clear();
    TA_flockMembersVec.resize(len);
    for(int i=0; i<len; i++)
    {
        TA_flockMembersVec[i] = memberList[i];
    }

}
void TAFlockButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        onClickRightButton();
        return;
    }
    QPushButton::mousePressEvent(event);
}
void TAFlockButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
//        emit leftDoubleClickSignal();
        onDoubleClickFlockButton();
        return;
    }
    QPushButton::mouseDoubleClickEvent(event);
}
void TAFlockButton::creatMenu()
{
    TA_menu = new QMenu(this);
    QAction *sendMessage = new QAction(tr("发送群消息"), TA_menu);
    QAction *showRecord = new QAction(tr("查看消息记录"), TA_menu);
    QAction *showInfo = new QAction(tr("查看资料"), TA_menu);

    connect(sendMessage, SIGNAL(triggered()),this, SLOT(onDoubleClickFlockButton()));
    connect(showRecord, SIGNAL(triggered()),this, SLOT(onClickShowRecord()));
    connect(showInfo, SIGNAL(triggered()),this, SLOT(onClickShowInformation()));
    TA_menu->addAction(sendMessage);
    TA_menu->addAction(showRecord);
    TA_menu->addAction(showInfo);
    TA_menu->addSeparator();

    if (TA_mainWidget == NULL)
        return;

    if (TA_mainWidget->getLocalMyInformation().TA_userID == TA_flockInfo.TA_creatorID)
    {
        QAction *dropFlock = new QAction(tr("解散该群"), TA_menu);
        connect(dropFlock, SIGNAL(triggered()),
                this, SLOT(onClickDropFlock()));
        TA_menu->addAction(dropFlock);

        QAction *addMember = new QAction(tr("添加群成员"), TA_menu);
        connect(addMember, SIGNAL(triggered()),
                this, SLOT(onClickAddMemberFlock()));
        TA_menu->addAction(addMember);
    }
    else
    {
        QAction *leaveFlock = new QAction(tr("退出该群"), TA_menu);
        connect(leaveFlock, SIGNAL(triggered()),
            this, SLOT(onClickLeaveFlock()));
        TA_menu->addAction(leaveFlock);
    }
}
