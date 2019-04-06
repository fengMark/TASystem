#include "tamainwidget.h"
#include "ui_tamainwidget.h"
#include<QToolButton>
#include <QWidget>
#include<QListWidget>
#include<QLineEdit>
#include<QInputDialog>
#include<QMessageBox>
#include<taclicklabel.h>
#include<control/tamainctl.h>
#include<model/taencryption.h>
#include<tatoolitem.h>
#include<taflockbutton.h>
#include<tafriendbutton.h>
#include<taflockwidget.h>
#include<tainformationwidget.h>
#include<taflockinformationwidget.h>
#include<tafriendlistwidget.h>
#include<tasearchlistwidget.h>
#include<taflocklistwidget.h>
#include<talatestmessagelistwidget.h>
#include<talinkfriendbutton.h>
#include<talinkflockbutton.h>
#include<talinkbutton.h>
#include<taflockwidget.h>
#include<tachatwidget.h>
#include<QCloseEvent>
#include<QMouseEvent>
#include<QTimerEvent>
#include<QLabel>
#include<QMenu>
#include<QWidget>
#include<QDebug>
TAMainWidget::TAMainWidget(const UserInformation me,QWidget *parent) :
    TAdatabase(me.TA_userID),QWidget(parent),
    ui(new Ui::TAMainWidget)
{
    ui->setupUi(this);
    TAflag = 0;
    TAtimerId = 0;
    TAmyself = me;
    TAonlineCounter = 0;
    initTAMainWidget();
    linkSignalWithSlot();
    creatMenu();
    setWindowTitle(tr("TA"));

}
UserInformation TAMainWidget::getLocalMyInformation()const{
    return TAmyself;
}
void TAMainWidget::dropFlock(const QString &flockID){
    if (NULL != TAmainCtl)
        TAmainCtl->dropFlock(TAmyself.TA_userID, flockID);
}
void TAMainWidget::leaveFlock(const QString &memberID, const QString &flockID){
    if (NULL != TAmainCtl)
        TAmainCtl->leaveFlock(memberID, flockID);
}
void TAMainWidget::removeFlockInformationWidget(const QString &flockID){
    TAflockInformationMap[flockID]->deleteLater();
    TAflockInformationMap.remove(flockID);
}
void TAMainWidget::removeFlockChatWidget(const QString &flockID){
    TAflockRoomMap[flockID]->deleteLater();
    TAflockRoomMap.remove(flockID);
}
void TAMainWidget::uploadIconFile(const QString &icon_name){
    TAmainCtl->uploadIconFile(icon_name,TAmyself.TA_userID);
}
bool TAMainWidget::isMyFriend(const QString &id) const{
    if (TAfriendMap.contains(id))
        return true;
    return false;
}
void TAMainWidget::changeFlockInformation(const FlockInformation &flock){
    if (NULL != TAmainCtl)
        TAmainCtl->changeFlockInformation(flock);
}
void TAMainWidget::changeFlockMemberRemark(const TempStrings &tmpStr){
    if (NULL != TAmainCtl)
        TAmainCtl->changeFlockMemberRemark(tmpStr);
}
void TAMainWidget::addFriend(const QString friendID){
    if (0 == friendID.compare(TAmyself.TA_userID))
    {
        QMessageBox::critical(NULL, tr("添加失败"), tr("不能添加自己"));
        return;
    }
    if (TAfriendMap.contains(friendID))
    {
        QMessageBox::critical(NULL, tr("添加失败"), tr("该用户已经是您的好友"));
        return;
    }

    bool isOkMes = false;
    QString checkMes = QInputDialog::getText(NULL, "添加好友",
                                           "请输入验证信息",
                                           QLineEdit::Normal,
                                           QString(tr("你好,我是: %1")).
                                             arg(TAmyself.TA_nickname),
                                           &isOkMes);
    if (!isOkMes)
        return;

    TalkMessage mes;
    mes.TA_senderID = TAmyself.TA_userID;
    mes.TA_receiverID = friendID;
    mes.TA_text = checkMes;
    mes.TA_type = REQUEST_FRIEND;

    if (NULL != TAmainCtl)
        TAmainCtl->addFriend(mes);
}
TAMainWidget::~TAMainWidget()
{
    delete ui;
    // 关闭定时器
    if (TAtimerId != 0)
    {
        killTimer(TAtimerId);
        TAtimerId = 0;
    }
    if (TAdatabase.isOpen())
    {
        TAdatabase.close();
    }
    // 清空容器
    clearContainers();
    qDeleteAll(TAchatRoomMap.begin(), TAchatRoomMap.end());
    TAchatRoomMap.clear();

    qDeleteAll(TAflockRoomMap.begin(), TAflockRoomMap.end());
    TAflockRoomMap.clear();

    qDeleteAll(TAinformationMap.begin(), TAinformationMap.end());
    TAinformationMap.clear();

    qDeleteAll(TAflockInformationMap.begin(), TAflockInformationMap.end());
    TAflockInformationMap.clear();

}
void TAMainWidget::removeInformationWidget(const QString & userID)
{
    TAinformationMap[userID]->deleteLater();
    TAinformationMap.remove(userID);
}
void TAMainWidget::changeMyInformation(const UserInformation &me)
{
    if (NULL != TAmainCtl)
        TAmainCtl->changeMyInformation(me);
}
void TAMainWidget::initTAMainWidget()
{
    TAmainCtl=new TAMainCtl(TAmyself.TA_userID);
    headlabel=new TAClickLabel(this);
    headlabel->setFixedSize(HEAD_BIG_SIZE,HEAD_BIG_SIZE);
    QString str = QString(":/image/resource/image/head/%1.bmp").arg(QString::number(TAmyself.TA_headPortrait));
    headlabel->setPixmap(QPixmap(str));
    nicklabel=new QLabel;
    nicklabel->setText(QString("%1\n%2").arg(TAmyself.TA_nickname, TAmyself.TA_userID));
    ui->headhorizontalLayout->addWidget(headlabel);
    ui->headhorizontalLayout->addWidget(nicklabel);
    ui->headhorizontalLayout->addStretch();

    ui->comboBox->addItem(tr("在线"));
    ui->comboBox->addItem(tr("隐身"));
    ui->comboBox->addItem(tr("忙碌"));
    ui->comboBox->addItem(tr("离开"));    
    ui->comboBox->addItem(tr("请勿打扰"));
    ui->comboBox->setCurrentIndex(TAmyself.TA_status);



    ui->searchlabel->setStyleSheet("border: 1px;border-radius:3px; border-style: solid;border-color: gray;background: white;");
    ui->searchlabel->setFixedHeight(25);

    TAsearchline=new QLineEdit(this);
    TAsearchline->setStyleSheet("border: 0px;");
    TAsearchline->setPlaceholderText(tr("search friends"));

    TAtoolBtnsearch = new QToolButton();
    TAtoolBtnsearch->setIcon(QIcon(":/image/resource/image/search.png"));
    TAtoolBtnsearch->setStyleSheet("border: 0px;");
    TAtoolBtnsearch->setEnabled(false);


    QHBoxLayout *layoutTopSearch = new QHBoxLayout;
    layoutTopSearch->setContentsMargins(0, 0, 0, 0);
    layoutTopSearch->addWidget(TAsearchline);
    layoutTopSearch->addWidget(TAtoolBtnsearch);
    ui->searchlabel->setLayout(layoutTopSearch);


    TAsearchlistWidget = new TASearchListWidget;
    TAsearchlistWidget->setVisible(false);
    ui->verticalLayout->addWidget(TAsearchlistWidget);

    TAfriendlistWidget= new TAFriendListWidget(this, this);
    TAflocklistWidget = new TAFlockListWidget(this, this);
    TAmessagelistWidget = new TALatestMessageListWidget(this, this);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->addTab(TAfriendlistWidget, tr("联系人"));
    ui->tabWidget->addTab(TAflocklistWidget,tr("群"));
    ui->tabWidget->addTab(TAmessagelistWidget, tr("新消息(0)"));

    ui->labelOnlineNum->setHidden(true);

    ui->toolBtnNewMes->setIcon(QIcon(":/image/resource/image/noMessage.png"));
    ui->toolBtnNewMes->setAutoRaise(true);
    ui->toolBtnNewMes->setToolTip(tr("无新消息"));
    ui->toolBtnNewMes->setStatusTip(tr("无最新消息"));
    ui->toolBtnNewMes->setHidden(true);

    ui->addFlockBtn->setIcon(QIcon(":/image/resource/image/addFlock.png"));
    ui->addFlockBtn->setAutoRaise(true);
    ui->addFlockBtn->setToolTip(tr("加群"));
    ui->addFlockBtn->setStatusTip(tr("添加群"));


    ui->settingBtn->setIcon(QIcon(":/image/resource/image/config.png"));
    ui->settingBtn->setAutoRaise(true);
    ui->settingBtn->setToolTip(tr("系统管理"));
    ui->settingBtn->setStatusTip(tr("打开系统管理"));


    ui->addFriendBtn->setIcon(QIcon(":/image/resource/image/addFriend.png"));
    ui->addFriendBtn->setAutoRaise(true);
    ui->addFriendBtn->setToolTip(tr("加好友"));
    ui->addFriendBtn->setStatusTip(tr("添加好友"));

    setMinimumSize(278,300);
    resize(278,500);
}

void TAMainWidget::setOnlineCounter(int num)
{
    TAonlineCounter = num;
    ui->labelOnlineNum->setText(QString(tr("在线人数: %1")).arg(QString::number(num)));

}
void TAMainWidget::searchFriend(const QString & text)
{
    TAsearchlistWidget->setVisible(true);
    ui->tabWidget->setVisible(false);
    if (text.isEmpty())
    {
        onClickSearchFriend();
        return;
    }
    TAtoolBtnsearch->setIcon(QIcon(":/image/resource/image/cancel.png"));
    TAtoolBtnsearch->setEnabled(true);

    TAsearchlistWidget->clearAll();
    QMapIterator<QString, TAFriendButton *> i(TAfriendMap);
     while (i.hasNext()) {
         i.next();
         if(i.key().contains(text)|| i.value()->getInformation().TA_nickname.contains(text)|| i.value()->getInformation().TA_remarkName.contains(text))
         {
             TALinkFriendButton * btn = new TALinkFriendButton(TAfriendMap[i.key()], this);
             connect(btn, SIGNAL(deleteLinkButton(QString)),this, SLOT(removeLinkButton(QString)));
             TAsearchlistWidget->addItem(btn);
         }
     }
}
void TAMainWidget::removeLinkButton(const QString & id)
{
    if (TAlinkMap.contains(id))
    {
        TAmessagelistWidget->removeItem(TAlinkMap[id]);
        TAlinkMap[id]->deleteLater();
        TAlinkMap.remove(id);
        ui->tabWidget->setTabText(2, QString(tr("新消息(%1)"))
                                .arg(TAmessagelistWidget->size()));
    }
    else
    {
        qDebug() << "there is some error in linkButton delete.";
    }
}
void TAMainWidget::onClickSearchFriend()
{
    TAtoolBtnsearch->setIcon(QIcon(":/image/resource/image/search.png"));
    TAtoolBtnsearch->setEnabled(false);
    TAsearchlistWidget->setVisible(false);
    ui->tabWidget->setVisible(true);
}

void TAMainWidget::receiveFriendTalkMessage(const TalkMessage &mes){
    if (TALK_FLOCK == mes.TA_type)
    {
        if (!TAflockMap.contains(mes.TA_receiverID))
            return;
        if (0 == mes.TA_senderID.compare(TAmyself.TA_userID))
            return;

        TAFlockWidget *flockRoom = NULL;
        if (TAflockRoomMap.contains(mes.TA_receiverID))
        {
            flockRoom = TAflockRoomMap[mes.TA_receiverID];
        }
        else
        {
            TAflockMap[mes.TA_receiverID]->openChatRoom();
            flockRoom = TAflockMap[mes.TA_receiverID]->getRoomPoint();
        }
        flockRoom->appendMessageShow(mes);

        if (!TAflockMap[mes.TA_receiverID]->isRoomShow())
        {
            // 创建 TALinkFlcokButton
            if (!TAlinkMap.contains(mes.TA_receiverID))
            {
                TALinkFlockButton * btn = new TALinkFlockButton(
                            TAflockMap[mes.TA_receiverID], this);
                connect(btn, SIGNAL(deleteLinkButton(QString)),
                        this, SLOT(removeLinkButton(QString)));
                TAlinkMap.insert(mes.TA_receiverID, btn);
                TAmessagelistWidget->addItem(btn);
                ui->tabWidget->setTabText(2, QString(tr("新消息%1"))
                                        .arg(TAmessagelistWidget->size()));
            }

             TAlinkMap[mes.TA_receiverID]->setLatestMessage(mes);
        }
        return;
    }
    TAChatWidget *chatRoom = NULL;

    if (!TAfriendMap.contains( mes.TA_senderID))
    {
        return;
    }

    if (TAchatRoomMap.contains(mes.TA_senderID))
    {
       chatRoom =  TAchatRoomMap[mes.TA_senderID];
    }
    else
    {
        TAfriendMap[mes.TA_senderID]->openChatRoom();
        chatRoom = TAfriendMap[mes.TA_senderID]->getRoomPoint();
    }

    switch (mes.TA_type)
    {
    case TALK_MESSAGE:
        chatRoom->appendMessageShow(mes);
        // 存储消息记录 至 数据库
        TAdatabase.addHistoryMessage(mes);
        if (!TAfriendMap[mes.TA_senderID]->isRoomShow())
        {
            // 创建 TALinkFriendButton
            if (!TAlinkMap.contains(mes.TA_senderID))
            {
                TALinkFriendButton * btn = new TALinkFriendButton(TAfriendMap[mes.TA_senderID], this);
                connect(btn, SIGNAL(deleteLinkButton(QString)),this, SLOT(removeLinkButton(QString)));
                TAlinkMap.insert(mes.TA_senderID, btn);
                TAmessagelistWidget->addItem(btn);
                ui->tabWidget->setTabText(2, QString(tr("新消息%1")).arg(TAmessagelistWidget->size()));
            }
            TAlinkMap[mes.TA_senderID]->setLatestMessage(mes);
        }

        break;
    case REQUEST_FILE:
    {
        chatRoom->pendingFile(mes);
        break;
    }
    case AGREE_FILE:
    {
        chatRoom->startSending(mes);
        break;
    }
    case REFUSE_FILE:
    {
        chatRoom->refusedSendFile();
        break;
    }
    }

//	//打开聊天窗口，并显示对方发来的信息
//	iter.value()->openChatRoom();
//	iter.value()->getRoomPoint()->setOutputLine(mes);

    //appendMessageShow();
}
void TAMainWidget::clearContainers(){
    TAfriendsGroupList.clear();

    TAindexFriendsGroupMap.clear();

    qDeleteAll(TAfriendMap.begin(), TAfriendMap.end());
    TAfriendMap.clear();

    qDeleteAll(TAtoolItemsFriendsVec.begin(), TAtoolItemsFriendsVec.end());
    TAtoolItemsFriendsVec.clear();


    qDeleteAll(TAflockMap.begin(), TAflockMap.end());
    TAflockMap.clear();



    qDeleteAll(TAlinkMap.begin(), TAlinkMap.end());
    TAlinkMap.clear();

}
void TAMainWidget::setMyInformation(const UserInformation &user){
    QString str = QString(":/image/resource/image/head/%1.bmp").arg(QString::number(user.TA_headPortrait));
    headlabel->setPixmap(QPixmap(str));
    nicklabel->setText(QString("%1\n%2").arg(user.TA_nickname, user.TA_userID));
}
void TAMainWidget::linkSignalWithSlot(){
    connect(TAmainCtl, SIGNAL(getFriendsSuccess(QVector<FriendInformation>)),
            this, SLOT(setFriendListWidget(QVector<FriendInformation>)));
    connect(TAmainCtl, SIGNAL(getFlocksSuccess(QVector<FlockInformation>)),
            this, SLOT(setFlockListWidget(QVector<FlockInformation>)));

    connect(TAmainCtl, SIGNAL(getFriendTalkMessage(TalkMessage)),
            this, SLOT(receiveFriendTalkMessage(TalkMessage)));

    connect(TAmainCtl, SIGNAL(getFriendChangedStatus(QString,int)),
            this, SLOT(setFriendStatus(QString,int)));
    connect(TAmainCtl, SIGNAL(getFriendChangedInformation(FriendInformation)),
            this, SLOT(setFriendInformation(FriendInformation)));
    connect(TAmainCtl, SIGNAL(getFriendInformationSuccess(UserInformation)),
            this, SLOT(showFriendInformation(UserInformation)));
    connect(headlabel, SIGNAL(clicked()),this, SLOT(getMyInformation()));

    connect(TAmainCtl, SIGNAL(getMyInformationSuccess(UserInformation)),
            this, SLOT(showMyInformation(UserInformation)));

    connect(TAmainCtl, SIGNAL(changeMyInformationSuccess(UserInformation)),
            this, SLOT(setMyInformation(UserInformation)));

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeMyStatus(int)));
    connect(TAmainCtl, SIGNAL(changeFriendRemarkSuccess(TempStrings)),
            this, SLOT(setFriendRemark(TempStrings)));
    connect(TAmainCtl, SIGNAL(deleteFriendSuccess(QString, QString, QString)),
            this, SLOT(deletedFriend(QString, QString, QString)));

    connect(TAmainCtl, SIGNAL(getFriendRequest(TalkMessage, UserInformation)),
            this, SLOT(receiveFriendRequest(TalkMessage, UserInformation)));


    connect(TAmainCtl, SIGNAL(getFlockRequest(TalkMessage, FlockMember, FlockInformation)),
            this, SLOT(receiveFlockRequest(TalkMessage, FlockMember, FlockInformation)));
    connect(TAmainCtl, SIGNAL(dropFlockSuccess(QString)),
            this, SLOT(dropedFlock(QString)));
    connect(TAmainCtl, SIGNAL(leaveFlockSuccess(QString,QString,QString)),
            this, SLOT(leavedFlock(QString,QString,QString)));
    connect(TAmainCtl, SIGNAL(haveMemberleavedFlock(QString,QString,QString)),
            this, SLOT(leavedFlock(QString,QString,QString)));
    connect(TAmainCtl, SIGNAL(changeFlockSuccess(FlockInformation)),
            this, SLOT(setFlockInformation(FlockInformation)));
    connect(TAmainCtl, SIGNAL(changeFlockRemarkSuccess(TempStrings)),
            this, SLOT(setFlockMemberRemark(TempStrings)));
    connect(TAmainCtl, SIGNAL(createFlockSuccess(FlockInformation)),
            this, SLOT(createFlockSuccess(FlockInformation)));



    connect(TAmainCtl, SIGNAL(closeWindowSignal()),
            this, SLOT(closeWindow()));

    connect(TAsearchline, SIGNAL(textChanged(QString)),
            this, SLOT(searchFriend(QString)));
    connect(TAtoolBtnsearch, SIGNAL(clicked()),
           this, SLOT(onClickSearchFriend()));

    connect(ui->addFriendBtn, SIGNAL(clicked()),
            this, SLOT(onClickAddFriend()));
    connect(ui->addFlockBtn, SIGNAL(clicked()),
            this, SLOT(onClickAddFlock()));
    connect(ui->settingBtn, SIGNAL(clicked()),
            this, SLOT(onClickRightButton()));
    connect(ui->toolBtnNewMes, SIGNAL(clicked()),
            this, SLOT(showLatestMessageWidget()));

    connect(TAmainCtl, SIGNAL(renameBoxSuccess(QString,QString)),
            this, SLOT(renameBoxSuccess(QString, QString)));
    connect(TAmainCtl, SIGNAL(moveFriendToBoxSuccess(QString,QString,QString)),
            this, SLOT(moveFriendToBoxSuccess(QString,QString,QString)));

    connect(TAmainCtl, SIGNAL(getNetworkHistoryMessagesSuccess(QString,QDate,QVector<TalkMessage>)),
            this, SLOT(setNetworkMessageWidget(QString,QDate,QVector<TalkMessage>)));

    connect(TAmessagelistWidget, SIGNAL(timerStatus(bool)),
            this, SLOT(setTimerStatus(bool)));
}
void TAMainWidget::createFlock(){
    bool isOk = false;
    QString flockName = QInputDialog::getText(NULL, "创建群","请输入群名",QLineEdit::Normal,QString("%1的TA群").arg(TAmyself.TA_nickname),&isOk);
    if (!isOk)
        return;

    isOk = false;
    QString flockTheme = QInputDialog::getText(NULL, "创建群","请输入群主题",QLineEdit::Normal,tr("没有主题"), &isOk);
    if (!isOk)
        return;

    TempStrings tmpStr;
    tmpStr.TA_one = TAmyself.TA_userID;
    tmpStr.TA_two = flockName;
    tmpStr.TA_three = flockTheme;

    if (NULL != TAmainCtl)
        TAmainCtl->createFlock(tmpStr);
}
void TAMainWidget::showLatestMessageWidget(){
    if (TAmessagelistWidget == NULL)
    {
        return;
    }
    ui->tabWidget->setCurrentWidget(TAmessagelistWidget);
}
void TAMainWidget::setTimerStatus(bool isOpen){
    if (isOpen)
    {
        if (0 == TAtimerId)
        {
            ui->toolBtnNewMes->setHidden(false);
            TAtimerId=startTimer(400);//400ms
            ui->toolBtnNewMes->setStatusTip(tr("查阅最新消息"));
            ui->toolBtnNewMes->setToolTip(QString(tr("有新消息")));
        }
    }

    else if (!isOpen && TAtimerId != 0)
    {
        killTimer(TAtimerId);
        TAtimerId = 0;
        ui->toolBtnNewMes->setToolTip(tr("无新消息"));
        ui->toolBtnNewMes->setStatusTip(tr("无最新消息"));
        ui->toolBtnNewMes->setHidden(true);
    }
}
void TAMainWidget::leavedFlock(const QString & memberID,const QString & flockID,const QString & creatorID)
{
    if (creatorID == TAmyself.TA_userID)
    {
        QMessageBox::information(NULL, tr("群消息"),
                                 QString(tr("用户%1已经退出群%2")).
                                 arg(memberID, flockID));

        // 如果群是打开的，最好及时刷新，将成员在群中删除
        // 不然要等到下次打开群的时候，才能看到成员已被删除
        // 关系也不大
        if (TAflockRoomMap.contains(flockID))
        {
            TAflockRoomMap[flockID]->removeFlockMemberButton(memberID);
        }
        if (TAflockMap.contains(flockID))
        {
            TAflockMap[flockID]->deleteFlockMember(memberID);
        }
    }
    else if (memberID == TAmyself.TA_userID)
    {
        QMessageBox::information(NULL, tr("群消息"),
                                 QString(tr("您好,%1,您已经退出群%2")).
                                 arg(memberID, flockID));

        if (TAflockRoomMap.contains(flockID))
        {
            TAflockRoomMap[flockID]->close();
            TAflockRoomMap[flockID]->deleteLater();
            TAflockRoomMap.remove(flockID);
        }

        if (TAflockMap.contains(flockID))
        {
            TAflocklistWidget->removeItem( TAflockMap[flockID] );
            TAflockMap[flockID]->deleteLater();
            TAflockMap.remove(flockID);
        }
        qDebug() << "leaved flock";
    }
    else
    {
        if (TAflockRoomMap.contains(flockID))
        {
            TAflockRoomMap[flockID]->removeFlockMemberButton(memberID);
        }
        if (TAflockMap.contains(flockID))
        {
            TAflockMap[flockID]->deleteFlockMember(memberID);
        }
    }
}
void TAMainWidget::deletedFriend(const QString & myID, const QString & friendID,const QString & groupName)
{
    if (myID !=TAmyself.TA_userID)
        return;


    if (!TAfriendMap.contains(friendID))
        return;

    TAtoolItemsFriendsVec[TAindexFriendsGroupMap.value(groupName)]->removeItem(TAfriendMap[friendID]);
    TAfriendMap[friendID]->deleteLater();
    TAfriendMap.remove(friendID);

    if (TAchatRoomMap.contains(friendID))
    {
        TAchatRoomMap[friendID]->close();
        TAchatRoomMap[friendID]->deleteLater();
        TAchatRoomMap.remove(friendID);
    }
    setOnlineCounter(TAonlineCounter - 1);
}
void TAMainWidget::onClickAddFlock(){
    bool isOk = false;
    QString flockID = QInputDialog::getText(NULL, "添加群","请输入群帐号", QLineEdit::Normal,"", &isOk);
    if (!isOk)
        return;

    // mark
    // 检验 是否全是 数字 或者 一开始就限制 或者 重写一个messagebox

    if (TAflockMap.contains(flockID))
    {
        QMessageBox::critical(NULL, tr("添加失败"), tr("你已经加入该群"));
        return;
    }
    bool isOkMes = false;
    QString checkMes = QInputDialog::getText(NULL, "添加群","请输入验证信息",QLineEdit::Normal, QString(tr("你好,我是: %1")).arg(TAmyself.TA_nickname),&isOkMes);
    if (!isOkMes)
        return;

    TalkMessage mes;
    mes.TA_senderID = TAmyself.TA_userID;
    mes.TA_receiverID = flockID;
    mes.TA_text = checkMes;
    mes.TA_type = REQUEST_FLOCK;

    if (NULL != TAmainCtl)
        TAmainCtl->addFlock(mes);
}
void TAMainWidget::onClickRightButton(){
    if (TAmenu == NULL)
    {
        creatMenu();
    }
    TAmenu->exec(QCursor::pos() - QPoint(105, 125));
}
void TAMainWidget::onClickAddFriend(){
    bool isOk = false;
    QString friendID = QInputDialog::getText(NULL, "添加好友","请输入对方帐号",QLineEdit::Normal,"", &isOk);
    if (!isOk)
        return;
    if (0 == friendID.compare(TAmyself.TA_userID))
    {
        QMessageBox::critical(NULL, tr("添加失败"), tr("不能添加自己"));
        return;
    }
    if (TAfriendMap.contains(friendID))
    {
        QMessageBox::critical(NULL, tr("添加失败"), tr("该用户已经是您的好友"));
        return;
    }
    bool isOkMes = false;
    QString checkMes = QInputDialog::getText(NULL, "添加好友","请输入验证信息",QLineEdit::Normal,QString(tr("你好,我是: %1")).arg(TAmyself.TA_nickname),&isOkMes);
    if (!isOkMes)
        return;
    TalkMessage mes;
    mes.TA_senderID = TAmyself.TA_userID;
    mes.TA_receiverID = friendID;
    mes.TA_text = checkMes;
    mes.TA_type = REQUEST_FRIEND;

    if (NULL != TAmainCtl)
        TAmainCtl->addFriend(mes);
}
void TAMainWidget::onClickChangePwd(){
    bool isOkMes = false;
    QString oldPwd = QInputDialog::getText(NULL, "修改密码","请输入原密码:",QLineEdit::Password,NULL, &isOkMes);
    if (!isOkMes)
        return;

    while(1)
    {
        QString newPwdOne = QInputDialog::getText(NULL, "修改密码","请输入新密码:",QLineEdit::Password,NULL,&isOkMes);
        if (!isOkMes)
            return;
        if (newPwdOne.length() < 6)
        {
            QMessageBox::critical(NULL, tr("修改密码"),tr("新密码长度不能少于6个字符！"));
            continue;
        }
        if (newPwdOne.length() > 14)
        {
            QMessageBox::critical(NULL, tr("修改密码"),tr("新密码长度不能超过14个字符！"));
            continue;
        }

        QString newPwdTwo = QInputDialog::getText(NULL, "修改密码","请再次输入新密码:",QLineEdit::Password, NULL,&isOkMes);
        if (!isOkMes)
            return;
        if (newPwdTwo != newPwdOne)
        {
            QMessageBox::critical(NULL, tr("修改密码"), tr("两次新密码不一致！"));
            continue;
        }
        else
        {
            if (NULL != TAmainCtl)
            {
                QString oldPassword = TAEncryption::getInstace().getXorEncryptDecrypt(oldPwd, 10);
                QString newPassword = TAEncryption::getInstace().getXorEncryptDecrypt(newPwdOne, 10);
                TAmainCtl->requestChangePwd(oldPassword, newPassword);
                break;
            }
        }

    }
}
void TAMainWidget::receiveFlockRequest(const TalkMessage & mes,const FlockMember & member,const FlockInformation &flock)
{
    TalkMessage returnMes;
    switch (mes.TA_type)
    {
    case REQUEST_FLOCK:
    {
        QMessageBox::StandardButton rb = QMessageBox::question(NULL, tr("用户请求"),QString(tr("是否同意用户%1加入群%2?\n验证消息:\n%3")).
                    arg(mes.TA_senderID, mes.TA_receiverID, mes.TA_text),
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        if(rb == QMessageBox::Yes)
        {
            returnMes.TA_type = FLOCK_AGREE;
        }
        else
        {
            returnMes.TA_type = FLOCK_REFUSE;
        }
        returnMes.TA_senderID = mes.TA_receiverID; // 群
        returnMes.TA_receiverID = mes.TA_senderID; // 用户
        if (NULL != TAmainCtl)
            TAmainCtl->resultOfFlockRequest(returnMes);
        break;
    }

    case AGREE_FLOCK:
    {
        if (!TAflockMap.contains(mes.TA_receiverID))
            return;
        if (!TAflockRoomMap.contains(mes.TA_receiverID))
            return;
        if (!TAflockRoomMap[mes.TA_receiverID]->addFlockMemberButton(member))
            return;

        TAflockMap[mes.TA_receiverID]->addFlockMember(member.TA_userID);

        QString temp = QString(tr("用户%1,成功加入群%2(%3).")).
                arg(mes.TA_senderID, flock.TA_flockID, flock.TA_flockName);
        QMessageBox::information(NULL, tr("群消息"), temp);
        break;
    }
    case REFUSE_FLOCK:
    {
        QString temp = QString(tr("您好,%1,用户%2,拒绝加入群%3\n的群.")).
                arg(TAmyself.TA_userID, mes.TA_senderID, mes.TA_receiverID);
        QMessageBox::information(NULL, tr("群消息"), temp);
        break;
    }
    case FLOCK_AGREE:
    {
        if (TAflockMap.contains(flock.TA_flockID))
            return;
        if (!addFlockButton(flock))
            return;
        QString temp = QString(tr("成功加入群%1(%2).")).
                arg(flock.TA_flockID, flock.TA_flockName);
        QMessageBox::information(NULL, tr("群消息"), temp);

        break;
    }
    case FLOCK_REFUSE:
    {
        QString temp = QString(tr("您好,%1,群%2\n的群主拒绝您的加入请求.")).
                arg(TAmyself.TA_userID, mes.TA_senderID);
        QMessageBox::information(NULL, tr("群消息"), temp);
        break;
    }
    case FLOCK_REQUEST:
    {
        QMessageBox::StandardButton rb = QMessageBox::question(
                    NULL, tr("群请求"),
                    QString(tr("是否同意加入群%1?\n验证消息:\n%2")).
                    arg(mes.TA_senderID, mes.TA_text),
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        if(rb == QMessageBox::Yes)
        {
            returnMes.TA_type = AGREE_FLOCK;
        }
        else
        {
            returnMes.TA_type = REFUSE_FLOCK;
        }
        returnMes.TA_senderID = mes.TA_receiverID; // 用户
        returnMes.TA_receiverID = mes.TA_senderID; // 群
        if (NULL != TAmainCtl)
            TAmainCtl->resultOfFlockRequest(returnMes);
        break;
    }
    case NEW_FLOCK_MEMBER:
    {
        if (!TAflockMap.contains(flock.TA_flockID))
            return;
        if (!TAflockRoomMap.contains(flock.TA_flockID))
            return;
        if (!TAflockRoomMap[flock.TA_flockID]->addFlockMemberButton(member))
            return;

        TAflockMap[flock.TA_flockID]->addFlockMember(member.TA_userID);
        break;
    }
    default:
        break;
    }
}
void TAMainWidget::setFriendStatus(const QString &id, int status){
    QMap<QString, TAFriendButton*>::iterator iter;
    iter = TAfriendMap.find(id);
    //如果好友列表中不存在这个人，就直接返回
    if(TAfriendMap.end() == iter)
        return;
    iter.value()->getInformation().TA_status = status;
    iter.value()->setButtonStatus();
}
void TAMainWidget::changeMyStatus(const int index){
    int status = ONLINE;
    switch (index)
    {
    case 0:
        status = ONLINE;
        break;
    case 1:
        status = INVISIBLE;
        break;
    case 2:
        status = BUSY;
        break;
    case 3:
        status = LEAVE;
        break;
    case 4:
        status = NOTDISTURB;
        break;
    default:
        status = ONLINE;
        break;
    }

    if (NULL != TAmainCtl)
        TAmainCtl->changeMyStatus(TAmyself.TA_userID,status);
}
void TAMainWidget::creatMenu(){
    TAmenu = new QMenu(this);
    QAction *addFriend = new QAction(tr("添加好友"), TAmenu);
    QAction *addFlock = new QAction(tr("添加群"), TAmenu);
    QAction *changePwd = new QAction(tr("修改密码"), TAmenu);
    QAction *information = new QAction(tr("个人信息"), TAmenu);

    connect(addFriend, SIGNAL(triggered()),
            this, SLOT(onClickAddFriend()));
    connect(addFlock, SIGNAL(triggered()),
            this, SLOT(onClickAddFlock()));
    connect(changePwd, SIGNAL(triggered()),
            this, SLOT(onClickChangePwd()));
    connect(information, SIGNAL(triggered()),
            this, SLOT(getMyInformation()));


    TAmenu->addAction(addFlock);
    TAmenu->addAction(addFriend);
    TAmenu->addAction(changePwd);
    TAmenu->addAction(information);
}
void TAMainWidget::getMyInformation(){
    if (NULL != TAmainCtl)
        TAmainCtl->getMyinformation(TAmyself.TA_userID);
}
void TAMainWidget::closeWindow(){
    TAflag = 1;
    closeAllWindows();
    TAmainCtl->closeConnect();
    close();
}
void TAMainWidget::closeEvent(QCloseEvent *event){
    if(continueToClose())
    {
        closeAllWindows();
        event->accept();
    }
    else
        event->ignore();
}
void TAMainWidget::renameBoxSuccess(const QString &title, const QString &newTitle){
    // 先修改： 组名 与 下标 映射表
    int index = TAindexFriendsGroupMap.value(title);
//    if (index == 0)
//    {
//        qDebug("cannot find this group");
//        return;
//    }
    TAindexFriendsGroupMap.remove(title);
    TAindexFriendsGroupMap.insert(newTitle, index);

    // 通过 存储每个分组的向量 获取分组 item ，再修改组名
    TAtoolItemsFriendsVec[index]->setTile(newTitle);

    TAfriendsGroupList[TAfriendsGroupList.indexOf(title)] = newTitle;

//    if (TAmessageManWidget != NULL)
//        TAmessageManWidget->setListModelGroup();
    refreshFriendButtonMenu();
}
void TAMainWidget::refreshFriendButtonMenu()
{
    QMapIterator<QString, TAFriendButton *> iter(TAfriendMap);
     while (iter.hasNext()) {
         iter.next();
         iter.value()->refreshMoveMenu();
     }
}
void TAMainWidget::renameBox(const QString &title){
    bool isOk = false;
    QString newTitle = QInputDialog::getText(NULL, "重命名分组","请输入新的分组名",QLineEdit::Normal,title,&isOk);
    if (!isOk)
        return;

    if (TAindexFriendsGroupMap.contains(newTitle))
    {
        QMessageBox::information(NULL, tr("重命名分组"), tr("失败，因为存在同名的组！"));
        return;
    }

    int index = TAindexFriendsGroupMap.value(title);

    if (TAtoolItemsFriendsVec[index]->getSize() <= 0)
    {
        renameBoxSuccess(title, newTitle);
        refreshFriendButtonMenu();
        return;
    }

    if (0 != newTitle.compare(title))
    {
        TAmainCtl->renameBox(title, newTitle);
    }
}
void TAMainWidget::removeBox(const QString &title){
    QMessageBox::StandardButton returnBtn;

    returnBtn = QMessageBox::question(NULL, tr("删除分组"),
        QString(tr("你确定要删除分组(%1)吗？")).arg(title),
        QMessageBox::Yes | QMessageBox::No,  QMessageBox::No);

    if(QMessageBox::No == returnBtn)
        return;

    int index = TAindexFriendsGroupMap[title];

    if (TAtoolItemsFriendsVec[index]->getSize() > 0)
    {
        QMessageBox::information(NULL, tr("删除组"), tr("无法删除，因为该组里还有组员！"));
        return;
    }

    TAtoolItemsFriendsVec[index]->deleteLater();
    TAtoolItemsFriendsVec[index] = NULL;
    TAfriendsGroupList.removeAt(TAfriendsGroupList.indexOf(title));
    TAindexFriendsGroupMap.remove(title);
//    if (TAmessageManWidget != NULL)
//        TAmessageManWidget->setListModelGroup();
    refreshFriendButtonMenu();
}
void TAMainWidget::receiveFriendRequest(const TalkMessage &mes, const UserInformation &friendInf){
    if (0 != mes.TA_receiverID.compare(TAmyself.TA_userID))
        return;
    TalkMessage returnMes;
    switch (mes.TA_type)
    {
    case REQUEST_FRIEND:
    {
        QMessageBox::StandardButton rb = QMessageBox::question(
                    NULL, tr("好友请求"),
                    QString(tr("是否同意用户%1(%2)添加你为好友?\n验证消息:\n%3")).
                    arg(mes.TA_senderID, friendInf.TA_nickname, mes.TA_text),
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            returnMes.TA_type = AGREE_FRIEND;
        }
        else
        {
            returnMes.TA_type = REFUSE_FRIEND;
        }
        returnMes.TA_senderID = mes.TA_receiverID;
        returnMes.TA_receiverID = mes.TA_senderID;
        if (NULL != TAmainCtl)
            TAmainCtl->resultOfFriendRequest(returnMes);
        break;
    }
    case AGREE_FRIEND:
    {
        FriendInformation fri;
        fri.TA_headPortrait = friendInf.TA_headPortrait;
        fri.TA_userID = friendInf.TA_userID;
        fri.TA_nickname = friendInf.TA_nickname;
        fri.TA_status = friendInf.TA_status;
        if (!addFriendButton(fri))
            return;
        QString temp = QString(tr("成功添加好友%1(%2).")).arg(friendInf.TA_userID, friendInf.TA_nickname);
        QMessageBox::information(NULL, tr("消息"), temp);
        break;
    }
    case REFUSE_FRIEND:
    {
        QString temp = QString(tr("用户%1(%2)\n拒绝您的好友添加请求.")).
                arg(friendInf.TA_userID, friendInf.TA_nickname);
        QMessageBox::information(NULL, tr("消息"), temp);
        break;
    }
    case DELETE_FRIEND:
    {
        break;
    }
    }
}
void TAMainWidget::showFlockInformation(const FlockInformation &flock){
    if (TAflockInformationMap.contains(flock.TA_flockID))
    {
        TAflockInformationMap[flock.TA_flockID]->raise();
        TAflockInformationMap[flock.TA_flockID]->activateWindow();
        TAflockInformationMap[flock.TA_flockID]->show();
        return;
    }
    TAFlockInformationWidget *infWidget = new TAFlockInformationWidget(this);
    TAflockInformationMap.insert(flock.TA_flockID, infWidget);
    if (flock.TA_creatorID.compare(TAmyself.TA_userID) == 0)
        infWidget->setReadOnly(true, true);
    else
        infWidget->setReadOnly(true, false);

    infWidget->setFlockInformation(flock);
    infWidget->show();
}
void TAMainWidget::closeAllWindows()
{
    QMap<QString, TAChatWidget*>::iterator iterFriendChat;
    for(iterFriendChat=TAchatRoomMap.begin();
        iterFriendChat!=TAchatRoomMap.end();
        ++iterFriendChat)
    {
//        TAChatWidget * widget = iterFriendChat.value();
//        if ( widget!= NULL)
//        {
//            iterFriendChat.value()->close();
        iterFriendChat.value()->deleteLater();

//            widget->close();
//            widget->deleteLater();
//            widget = NULL;
//        }
    }

    QMap<QString, TAFlockWidget*>::iterator iterFlockChat;
    for(iterFlockChat=TAflockRoomMap.begin();
        iterFlockChat!=TAflockRoomMap.end();
        ++iterFlockChat)
    {
        iterFlockChat.value()->deleteLater();
    }


    QMap<QString, TAInformationWidget*>::iterator iterInformation;
    for(iterInformation=TAinformationMap.begin();
        iterInformation!=TAinformationMap.end();
        ++iterInformation)
    {
        iterInformation.value()->deleteLater();
    }

    QMap<QString, TAFlockInformationWidget*>::iterator iterFlockInformation;
    for(iterFlockInformation=TAflockInformationMap.begin();
        iterFlockInformation!=TAflockInformationMap.end();
        ++iterFlockInformation)
    {
        iterFlockInformation.value()->deleteLater();
    }
}
bool TAMainWidget::continueToClose(){
    if (TAflag == 1)
    {
        return true;
    }
    if (TAchatRoomMap.size() != 0|| TAflockRoomMap.size() != 0)
    {
        QString temp = tr("还有聊天窗口没有关闭，您确定想要退出程序？");
        QMessageBox::StandardButton rb;
        rb = QMessageBox::question(this, tr("退出程序"), temp,
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if(rb == QMessageBox::Yes)
                return true;
        else
            return false;
     }
    return true;
}
void TAMainWidget::timerEvent(QTimerEvent *event){
    static bool show = true;
    if (event->timerId() == TAtimerId)
    {
        if (show)
            ui->toolBtnNewMes->setIcon(QIcon(":/image/resource/image/newMessage.png"));
        else
            ui->toolBtnNewMes->setIcon(QIcon(":/image/resource/image/noMessage.png"));
        show = !show;
    }
}
void TAMainWidget::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::RightButton)
    {
        return;
    }
    QWidget::mousePressEvent(event);
}
void TAMainWidget::getFriendsInformation(){
    if (NULL != TAmainCtl)
        TAmainCtl->getFriendsInformation(TAmyself.TA_userID, TAmyself.TA_status);
}
void TAMainWidget::getFriendInformation(const QString &id){
    if (NULL != TAmainCtl)
        TAmainCtl->getFriendInformation(id);
}
void TAMainWidget::setFlockInformation(const FlockInformation &flock){
    if (TAflockMap.contains(flock.TA_flockID))
    {
        TAflockMap[flock.TA_flockID]->setChangedInformation(flock);
    }
    if (TAflockRoomMap.contains(flock.TA_flockID))
    {
        TAflockRoomMap[flock.TA_flockID]->setChatInformation(flock);
    }
}
void TAMainWidget::showFriendInformation(const UserInformation &user){
    if (TAinformationMap.contains(user.TA_userID))
    {
        TAinformationMap[user.TA_userID]->setUserInformation(user);
        TAinformationMap[user.TA_userID]->raise();
        TAinformationMap[user.TA_userID]->activateWindow();
        TAinformationMap[user.TA_userID]->show();
        return;
    }
    TAInformationWidget *infWidget = new TAInformationWidget(this);
    TAinformationMap.insert(user.TA_userID, infWidget);
    infWidget->setReadOnly(true, false);
    infWidget->setUserInformation(user);
    infWidget->show();
}
void TAMainWidget::showMyInformation(const UserInformation &user){
    TAmyself = user;
    if (TAinformationMap.contains(TAmyself.TA_userID))
    {
        TAinformationMap[TAmyself.TA_userID]->raise();
        TAinformationMap[TAmyself.TA_userID]->activateWindow();
        TAinformationMap[TAmyself.TA_userID]->show();
        return;
    }

    TAInformationWidget *infWidget = new TAInformationWidget(this);
    TAinformationMap.insert(TAmyself.TA_userID, infWidget);
    infWidget->setReadOnly(true, true);
    infWidget->setUserInformation(user);
    infWidget->show();
}
void  TAMainWidget::setFriendRemark(const TempStrings &tempStr){
    if (tempStr.TA_one != TAmyself.TA_userID)
        return;
    QMap<QString, TAFriendButton*>::iterator iter;
    iter = TAfriendMap.find(tempStr.TA_two);
    if(TAfriendMap.end() == iter)
        return;

    iter.value()->getInformation().TA_remarkName = tempStr.TA_three;
    iter.value()->setButtonStatus();
}
void TAMainWidget::changeFriendRemark(const QString &friendID, const QString &remark){
    if (NULL != TAmainCtl)
        TAmainCtl->changeFriendRemark(TAmyself.TA_userID, friendID, remark);
}
void TAMainWidget::setFriendListWidget(const QVector<FriendInformation> &friendsVec){
    int num = 0;
    int len = friendsVec.size();
    for(int i=0; i<len; i++)
    {
        addFriendButton(friendsVec[i]);
        if (OFFLINE != friendsVec[i].TA_status && INVISIBLE != friendsVec[i].TA_status)
            num++;
    }
    setOnlineCounter(num);
    getFlocksInformation();
}
void TAMainWidget::setFlockListWidget(const QVector<FlockInformation> & flocksVec)
{
    int len = flocksVec.size();
    for(int i=0; i<len; i++)
    {
        addFlockButton(flocksVec[i]);
    }
}
bool TAMainWidget::addFlockButton(const FlockInformation & flcokInfo)
{
    if (TAflockMap.contains(flcokInfo.TA_flockID))
        return false;

    TAFlockButton *button = new TAFlockButton(flcokInfo, this, this);
    TAflockMap.insert(flcokInfo.TA_flockID, button);
    TAflocklistWidget->addItem(button);

    return true;
}
void TAMainWidget::insertChatRoomMap(const QString & id, TAChatWidget *chatRoom){
    TAchatRoomMap.insert(id, chatRoom);
}
void TAMainWidget::insertFlockRoomMap(const QString &id, TAFlockWidget *chatRoom){
    TAflockRoomMap.insert(id, chatRoom);
}
void TAMainWidget::getFlocksInformation(){
    if (NULL != TAmainCtl)
        TAmainCtl->getFlocksInformation(TAmyself.TA_userID);
}
void TAMainWidget::deleteFriend(const QString &friendID){
    if (NULL != TAmainCtl)
        TAmainCtl->deleteFriend(TAmyself.TA_userID, friendID);
}
void TAMainWidget::moveFriendToBox(const QString &friendID, const QString &groupName, const QString &title){
    if (NULL != TAmainCtl)
        TAmainCtl->moveFriendToBox(friendID, groupName, title);
}
void TAMainWidget::removeChatWidget(const QString &friendID){
    TAchatRoomMap[friendID]->deleteLater();
    TAchatRoomMap.remove(friendID);
}
bool TAMainWidget::addFriendButton(const FriendInformation &friInfo){
    if (TAfriendMap.contains(friInfo.TA_userID))
        return false;
    QString groupName = friInfo.TA_groupName;
    if (!TAfriendsGroupList.contains(groupName))//添加组
    {
        // 存储 新的分组号
        TAfriendsGroupList.append(groupName);

        // 新建并存储分组
        TAToolItem  *toolItem = new TAToolItem(groupName);

        connect(toolItem, SIGNAL(removeBoxSignal(QString)),this, SLOT(removeBox(QString)));
        connect(toolItem, SIGNAL(renameBoxSignal(QString)),this, SLOT(renameBox(QString)));

        TAtoolItemsFriendsVec.push_back(toolItem);
        TAfriendlistWidget->addItem(toolItem);
        TAindexFriendsGroupMap.insert(groupName, TAtoolItemsFriendsVec.size()-1);
    }
    TAFriendButton *button = new TAFriendButton(friInfo,&TAfriendsGroupList,this, this);
    TAtoolItemsFriendsVec[TAindexFriendsGroupMap.value(groupName)]->addItem(button);
    TAfriendMap.insert(friInfo.TA_userID, button);//添加按钮和ID的映射关系
    return true;

}
void TAMainWidget::getLatestMessage(){
    qDebug()<<"get offline message";
    if (NULL != TAmainCtl)
        TAmainCtl->getLatestMessage(TAmyself.TA_userID);
}
void TAMainWidget::setFlockMemberRemark(const TempStrings &tmpStr){
    if (TAflockRoomMap.contains(tmpStr.TA_one))
    {
        TAflockRoomMap[tmpStr.TA_one]->setFlockMemberRemark(tmpStr);
    }
}
void TAMainWidget::createFlockSuccess(const FlockInformation &flock){
    if (flock.TA_creatorID.compare(TAmyself.TA_userID) != 0)
        return;

    if (TAflockMap.contains(flock.TA_flockID))
        return;
    if (!addFlockButton(flock))
        return;
    QString temp =  QString(tr("您好,%1(%2).\n您已经成功创建群%3(%4).\n创建日期: %5.")).
                    arg(TAmyself.TA_userID, TAmyself.TA_nickname,
                        flock.TA_flockID, flock.TA_flockName,
                        flock.TA_createDate.toString("yyyy-MM-dd"));
    QMessageBox::information(NULL, tr("群消息"), temp);
}
void TAMainWidget::moveFriendToBoxSuccess(const QString & friendID, const QString & title,const QString & newTitle)
{
    if (!TAfriendMap.contains(friendID))
        return;
    TAtoolItemsFriendsVec[TAindexFriendsGroupMap.value(title)]->removeItem(TAfriendMap[friendID]);

    TAfriendMap[friendID]->getInformation().TA_groupName = newTitle;
    TAfriendMap[friendID]->refreshMoveMenu();
    TAtoolItemsFriendsVec[TAindexFriendsGroupMap.value(newTitle)]->addItem(TAfriendMap[friendID]);
}
void TAMainWidget::createBox(){
    bool isOk = false;
    QString groupName = QInputDialog::getText(NULL, "新建组","请输入分组名",QLineEdit::Normal,tr("new group"),&isOk);

    if (isOk)
    {
        if (TAindexFriendsGroupMap.contains(groupName))
        {
            QMessageBox::information(NULL, tr("新建组"), tr("无法创建，因为存在同名的组！"));
            return;
        }
        TAfriendsGroupList.append(groupName);

        TAToolItem *toolItem = new TAToolItem(groupName);

        connect(toolItem, SIGNAL(removeBoxSignal(QString)),this, SLOT(removeBox(QString)));
        connect(toolItem, SIGNAL(renameBoxSignal(QString)),this, SLOT(renameBox(QString)));

        TAtoolItemsFriendsVec.push_back(toolItem);
        TAfriendlistWidget->addItem(toolItem);

        // 存储映射关系
        TAindexFriendsGroupMap.insert(groupName, TAtoolItemsFriendsVec.size()-1);

//        if (TAmessageManWidget != NULL)
//            TAmessageManWidget->setListModelGroup();
        refreshFriendButtonMenu();
    }

}
void TAMainWidget::receiveMessagesFromChat(TalkMessage &mes)
{
    mes.TA_senderID=TAmyself.TA_userID;
    if (NULL != TAmainCtl)
        TAmainCtl->sendTalkMessage(mes);

}
