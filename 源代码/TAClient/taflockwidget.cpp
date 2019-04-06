#include "taflockwidget.h"
#include "ui_taflockwidget.h"
#include<QVector>
#include<QToolButton>
#include<taflockmemberbutton.h>
#include<taflockmemberlistwidget.h>
#include<QTextBrowser>
#include<taclicklabel.h>
#include<QLabel>
#include<QPushButton>
#include<tamainwidget.h>
#include<model/tatcpsocket.h>
TAFlockWidget::TAFlockWidget(const FlockInformation & flock,
                             TAMainWidget * mainWidget,
                             QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TAFlockWidget)
{
    ui->setupUi(this);
    m_mainWidget = mainWidget;
    m_flockInfor = flock;
    initTAFlockChatWidget();
    setLabelStatus();
    linkSignalWithSlot();
    setWindowFlags(this->windowFlags()&(~(Qt::WindowMinMaxButtonsHint|Qt::WindowMinimizeButtonHint)));
    setWindowTitle(tr("TA群聊窗口"));
}

TAFlockWidget::~TAFlockWidget()
{
    delete ui;
    qDeleteAll(m_flockMemberMap.begin(), m_flockMemberMap.end());
    m_flockMemberMap.clear();

    m_messagesVec.clear();
}
void TAFlockWidget::linkSignalWithSlot(){
    connect(ui->sendpushButton, SIGNAL(clicked()),
            this, SLOT(onClickBtnSend()));
    connect(ui->closepushButton, SIGNAL(clicked()),
            this, SLOT(onClickBtnClose()));
    connect(ui->cleartoolButton, SIGNAL(clicked()),
            this, SLOT(onClickBtnClear()));
}
void TAFlockWidget::initTAFlockChatWidget(){
    /*头部布局*/
    QHBoxLayout *h_headlayout=new QHBoxLayout;
    m_labelHead=new QLabel(this);
    m_labelInformation=new QLabel(this);
    h_headlayout->addWidget(m_labelHead);
    h_headlayout->addWidget(m_labelInformation);
    h_headlayout->addStretch();
    ui->headwidget->setLayout(h_headlayout);

    /*中间右侧布局*/
    QVBoxLayout *memberlayout = new QVBoxLayout;
    m_memberListWidget = new TAFlockMemberListWidget(this);
    memberlayout->addWidget(m_memberListWidget);
    ui->flockMemberwidget->setLayout(memberlayout);
}
void TAFlockWidget::setChatInformation(const FlockInformation &flock){
    m_flockInfor.TA_flockName = flock.TA_flockName;
    m_flockInfor.TA_theme = flock.TA_theme;
    setLabelStatus();
}
void TAFlockWidget::setFlockMemberRemark(const TempStrings &tmpStr){
    if (m_flockMemberMap.contains(tmpStr.TA_two))
    {
        m_flockMemberMap[tmpStr.TA_two]->setRemark(tmpStr);
    }
}
bool TAFlockWidget::removeFlockMemberButton(const QString &memberID){
    if (!m_flockMemberMap.contains(memberID))
        return false;
    m_flockMemberMap[memberID]->deleteLater();
    m_flockMemberMap.remove(memberID);
    return true;
}
void TAFlockWidget::closeEvent(QCloseEvent *event){
    emit roomQuitSignal();

    event->accept();
}
void TAFlockWidget::onClickBtnClose(){
    this->close();
}
void TAFlockWidget::onClickBtnClear(){
    ui->textBrowser->clear();
}
void TAFlockWidget::onClickBtnSend(){
    if (ui->textEdit->toPlainText().isEmpty())
        return;
    // 获取信息输入框的信息，并且更新信息输入框
    QString sendString = ui->textEdit->toHtml();
    ui->textEdit->clear();
    QString dateTime = TATcpSocket::getCurrentDateTime();
    QString temp = QString("<font size=\"FONT_SIEZE\" color=green>%1    %2: </font>%3")
        .arg("我").arg(dateTime).arg(sendString);
    ui->textBrowser->append(temp);

    m_mes.TA_text = sendString;
    m_mes.TA_type = TALK_FLOCK;
    m_mes.TA_receiverID = m_flockInfor.TA_flockID;
//    m_mes.m_senderID = m_chatInfor.m_;
//    m_mes.m_senderIP = TATcpSocket::getIP();

    emit sendMessagesFromChat(m_mes);
}
void TAFlockWidget::setFlockMemberList(const QVector<FlockMember> &memberList){
    int len = memberList.size();
    if (len <= 0) return;
    if (memberList[0].TA_flockID != m_flockInfor.TA_flockID)
        return;

    qDeleteAll(m_flockMemberMap.begin(), m_flockMemberMap.end());
    m_flockMemberMap.clear();

    for(int i=0; i<len; i++)
    {
        addFlockMemberButton(memberList[i]);
    }

    while(m_messagesVec.size() > 0)
    {
        appendMessageShow( m_messagesVec.front());
        m_messagesVec.pop_front();
    }
}
bool TAFlockWidget::addFlockMemberButton(const FlockMember &memInfo){
        if (0 != m_flockInfor.TA_flockID.compare(memInfo.TA_flockID))
            return false;
        if (m_flockMemberMap.contains(memInfo.TA_userID))
            return false;

        TAFlockMemberButton *button = new TAFlockMemberButton(memInfo, this, m_mainWidget, this);

        m_flockMemberMap.insert(memInfo.TA_userID, button);
        m_memberListWidget->addItem(button);

        return true;
}
void TAFlockWidget::setLabelStatus(){
    QString str = QString(":/image/resource/image/flock.png");
    QPixmap pixmap;
    QIcon icon(str);
    pixmap = icon.pixmap(QSize(HEAD_BIG_SIZE, HEAD_BIG_SIZE));
    m_labelHead->setPixmap(pixmap);

    QString  name = QString(tr("%1\n%2")).arg(m_flockInfor.TA_flockName,
                                              m_flockInfor.TA_theme);
    m_labelInformation->setText(name);
}
void TAFlockWidget::appendMessageShow(const TalkMessage &mes)

{
    //qDebug() << "flock receive mes:" << mes.m_senderID << m_flockInfor.m_flockID << mes.m_text;
    // using TalkMessage.m_senderID to find user's name
    if (m_flockMemberMap.size() == 0)
    {
        m_messagesVec.push_back(mes);
        return;
    }
    FlockMember member;
    if (!m_flockMemberMap.contains(mes.TA_senderID))
    {
        return;
    }
    member = m_flockMemberMap[mes.TA_senderID]->getInformation();

    QString  name;
    if(member.TA_remark.isEmpty())
        name = member.TA_nickname;
    else
        name = member.TA_remark;

    //将（收到的）信息显示在输出栏
    QString dateTime = TATcpSocket::getCurrentDateTime();
    QString temp = QString("<font size=\"FONT_SIEZE\" color=blue>%1    %2: </font>%3")
        .arg(name).arg(dateTime).arg(mes.TA_text);
    ui->textBrowser->append(temp);
}
