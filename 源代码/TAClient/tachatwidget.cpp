#include "tachatwidget.h"
#include "ui_tachatwidget.h"
#include<model/tareiverudpsockeet.h>
#include<model/tasendudpsocket.h>
#include<QDebug>
TAChatWidget::TAChatWidget(const ChatInformation &chatInf,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TAChatWidget)
{
    ui->setupUi(this);
    TA_chatInfor = chatInf;


    ui->filewidget->hide();
    TA_isStopped=true;
    setLabelStatus();
    setWidgetToolBtn();
    linkSignalWithSlot();
    setWindowFlags(this->windowFlags()&(~(Qt::WindowMinMaxButtonsHint|Qt::WindowMinimizeButtonHint)));
    setWindowTitle(tr("TA好友聊天窗口"));
}
void TAChatWidget::setWidgetToolBtn(){
    ui->filetoolButton->setIcon(QIcon(":/image/resource/image/folder.png"));
    ui->filetoolButton->setAutoRaise(true);
    ui->filetoolButton->setToolTip(tr("发送文件"));

    ui->cleartoolButton->setIcon(QIcon(":/image/resource/image/clear.png"));
    ui->cleartoolButton->setAutoRaise(true);
    ui->cleartoolButton->setToolTip(tr("清除聊天记录"));
}
void TAChatWidget::linkSignalWithSlot(){
    connect(ui->closepushButton, SIGNAL(clicked()),this, SLOT(onClickBtnClose()));
    connect(ui->sendpushButton,SIGNAL(clicked()),this,SLOT(onClickBtnSend()));
    connect(ui->cleartoolButton,SIGNAL(clicked()),this,SLOT(onClickBtnClear()));
    connect(ui->filetoolButton,SIGNAL(clicked()),this,SLOT(onClickBtnFile()));
}
TAChatWidget::~TAChatWidget()
{
    delete ui;
}
void TAChatWidget::onClickBtnFile(){
    if (ui->filewidget->isVisible())
    {
        return;
    }
    if (OFFLINE == TA_chatInfor.TA_friendStatus)
    {
        QMessageBox::critical(NULL, tr("发送文件"), tr("好友不在线，暂时无法发送"));
        return;
    }
    fileName = QFileDialog::getOpenFileName(this);

    QFile file(fileName);
    QString totalsize=QString::number(file.size());
    TA_Send_totalBytes=file.size();
    if(fileName.isEmpty())
    {
        QMessageBox::information(this,tr("警告"),tr("请选择文件"));
        return;
    }
    TA_fileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    ui->filenamelabel->setText(QString(tr("文件名: %1")).arg(TA_fileName));
    ui->progressBar->reset();
    ui->statuslabel->setText(tr("等待对方接受"));
    ui->filewidget->show();
    TA_mes.TA_type = REQUEST_FILE;
    TA_mes.TA_receiverID = TA_chatInfor.TA_friendID;
    TA_mes.TA_text = TA_fileName+"+"+totalsize;
    emit sendMessagesFromChat(TA_mes);
}
void TAChatWidget::initsender(){
    TA_isStopped = false;
    TA_has_Send=0;
    ui->progressBar->reset();
}
void TAChatWidget::initreceiver(){
    TA_isStopped = false;
    TA_has_Receive=0;
    ui->progressBar->reset();
}
void TAChatWidget::onClickBtnClear(){
    ui->textBrowser->clear();
}
void TAChatWidget::onClickBtnClose(){
    this->close();
}
void TAChatWidget::onClickBtnSend(){
    if (ui->textEdit->toPlainText().isEmpty())
        return;
    QString sendString = ui->textEdit->toHtml();
    ui->textEdit->clear();
    QDateTime dateTime = QDateTime::currentDateTime();
    QString temp = QString("<font size=\"FONT_SIEZE\" color=green>%1    %2: </font>%3")
        .arg("我").arg(dateTime.toString("yyyy-MM-dd hh:mm:ss")).arg(sendString);
    ui->textBrowser->append(temp);
    TA_mes.TA_text = sendString;
    TA_mes.TA_type = TALK_MESSAGE;
    TA_mes.TA_receiverID = TA_chatInfor.TA_friendID;
    TA_mes.TA_dateTime = dateTime;
    emit sendMessagesFromChat(TA_mes);
}
void TAChatWidget::appendMessageShow(const TalkMessage &mes){// 添加好友发来的信息
    QString  name;
    if(TA_chatInfor.TA_friendRemark.isEmpty())
        name = TA_chatInfor.TA_friendNickname;
    else
        name = TA_chatInfor.TA_friendRemark;

    QString temp = QString("<font size=\"FONT_SIEZE\" color=blue>%1    %2: </font>%3")
        .arg(name).arg(mes.TA_dateTime.toString("yyyy-MM-dd hh:mm:ss")).arg(mes.TA_text);
    ui->textBrowser->append(temp);
}
void TAChatWidget::pendingFile(const TalkMessage &mes){
    int btn = QMessageBox::information(this,tr("接受文件"),tr("来自%1(%2)的文件:\n%3\n是否接收？").arg(mes.TA_senderID).arg(mes.TA_senderIP).arg(mes.TA_text),QMessageBox::Yes,QMessageBox::No);
    if(btn == QMessageBox::Yes)
    {
        QStringList fileinfo=mes.TA_text.split("+");
        QString name = QFileDialog::getSaveFileName(0,tr("保存文件"),fileinfo.at(0));
        if(!name.isEmpty())
        {
                TA_mes.TA_type = AGREE_FILE;
                TA_mes.TA_receiverID = mes.TA_senderID;
                TA_mes.TA_receiverIP = mes.TA_senderIP;
                emit sendMessagesFromChat(TA_mes);


                QString size=fileinfo.at(1);
                TA_Receive_totalBytes=size.toInt();
                setReceiveFileName(name);

                initreceiver();
                TA_time.start();

                receiver=new TAReiverUdpSockeet(name,this);
                connect(receiver,SIGNAL(byread(qint64)),this,SLOT(updateReceiveProgress(qint64)));
                ui->filewidget->show();
        }
    }
    else
    {
        TA_mes.TA_type = REFUSE_FILE;
        TA_mes.TA_receiverID = mes.TA_senderID;
        TA_mes.TA_receiverIP = mes.TA_senderIP;
        emit sendMessagesFromChat(TA_mes);
    }
}
void TAChatWidget::setReceiveFileName(const QString &filename){
    TA_localFile = new QFile(filename);
    ui->filenamelabel->setText(filename.right(filename.size() - filename.lastIndexOf('/')-1));
}

void TAChatWidget::showReceiveFileProcess(){
    ui->statuslabel->setText(tr("准备接收。。。。"));
    connect(ui->cleartoolButton, SIGNAL(clicked()),
            this, SLOT(onClickBtnCancel()));
    connect(ui->closepushButton, SIGNAL(clicked()),
            this, SLOT(onClickBtnClose()));
}

void TAChatWidget::refusedSendFile(){
    showRefused();
}
void TAChatWidget::showRefused(){
    TA_isStopped = true;
    ui->statuslabel->setText(tr("对方拒绝接收！"));
}
void TAChatWidget::setLabelStatus(){
    QString str = QString(":/image/resource/image/head/%1.bmp").
        arg(QString::number(TA_chatInfor.TA_headPortrait));

    QString  name;
    if(TA_chatInfor.TA_friendRemark.isEmpty())
        name = TA_chatInfor.TA_friendNickname;
    else
        name = TA_chatInfor.TA_friendRemark;

    QPixmap pixmap;
    QIcon icon(str);
    pixmap = icon.pixmap(QSize(HEAD_BIG_SIZE, HEAD_BIG_SIZE));
    ui->labelhead->setPixmap(pixmap);
    ui->labelnickname->setText(name+'\n'+TA_chatInfor.TA_friendID);
}
void TAChatWidget::setChatInformation(const ChatInformation &chatInfo){
    if (chatInfo.TA_friendID != TA_chatInfor.TA_friendID)
        return;
    TA_chatInfor.TA_headPortrait = chatInfo.TA_headPortrait;
    TA_chatInfor.TA_friendNickname = chatInfo.TA_friendNickname;
    TA_chatInfor.TA_friendRemark = chatInfo.TA_friendRemark;
    TA_chatInfor.TA_friendMark = chatInfo.TA_friendMark;
    TA_chatInfor.TA_friendIP = chatInfo.TA_friendIP;
    TA_chatInfor.TA_friendStatus = chatInfo.TA_friendStatus;
}
void TAChatWidget::setFriendIP(const QString &ip){
    TA_chatInfor.TA_friendIP = ip;
}
void TAChatWidget::closeEvent(QCloseEvent *event){
        if (!TA_isStopped)
        {
            QString temp = tr("文件还在发送中，您确定想要关闭聊天窗口？");
            QMessageBox::StandardButton rb;
            rb = QMessageBox::question(this, tr("关闭聊天"), temp,QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if(rb == QMessageBox::No)
            {
                event->ignore();
                return;
            }
        }
    emit roomQuitSignal();
    event->accept();
}
void TAChatWidget::updateSendProgress(qint64 numBytes){
    qDebug()<<"has sended"<<numBytes;
    ui->progressBar->setMaximum(TA_Send_totalBytes);
    TA_has_Send += numBytes;
    ui->progressBar->setValue(TA_has_Send);

   float useTime = TA_time.elapsed();
   double speed = TA_has_Send / useTime;
   ui->statuslabel->setText(tr("已发送 %1MB (%2MB/s) \n共%3MB 已用时:%4秒\n估计剩余时间：%5秒")
                                  .arg(TA_has_Send / (1024*1024))      // 已发送
                                  .arg(speed*1000/(1024*1024),0,'f',2)  // 速度
                                  .arg(TA_Send_totalBytes / (1024 * 1024))      // 总大小
                                  .arg(useTime/1000,0,'f',0)            // 用时
                                  .arg(TA_Send_totalBytes/speed/1000 - useTime/1000,0,'f',0)); // 剩余时间
    if(TA_has_Send == TA_Send_totalBytes)
    {
        TA_isStopped = true;
        ui->statuslabel->setText(tr("传送文件成功"));
        emit SendFinished();
    }
}
void TAChatWidget::updateReceiveProgress(qint64 numBytes){
    float useTime = TA_time.elapsed();
    ui->progressBar->setMaximum(TA_Receive_totalBytes);
    TA_has_Receive+=numBytes;
    ui->progressBar->setValue(TA_has_Receive);
    double speed = TA_has_Receive/ useTime;
        ui->statuslabel->setText(tr("已接收 %1MB (%2MB/s) \n共%3MB 已用时:%4秒\n估计剩余时间：%5秒")
                                             .arg(TA_has_Receive / (1024*1024))   // 已接收
                                             .arg(speed*1000/(1024*1024),0,'f',2)  // 速度
                                             .arg(TA_Receive_totalBytes / (1024 * 1024))    // 总大小
                                             .arg(useTime/1000,0,'f',0)            // 用时
                                             .arg(TA_Receive_totalBytes/speed/1000 - useTime/1000,0,'f',0)); // 剩余时间

        if(TA_has_Receive == TA_Receive_totalBytes)
        {
            receiver->close();;
            ui->statuslabel->setText(tr("文件接收完毕"));
            emit ReceiveFinished();
        }
}
void TAChatWidget::showSendClosedConnect()
{
    ui->statuslabel->setText(tr("文件传送被中断！"));
    if(TA_has_Send == TA_Send_totalBytes){
        ui->statuslabel->setText(tr("传送文件成功"));
        TA_isStopped = true;
        delete sender;
    }
}
void TAChatWidget::showReceiveClosedConnect()
{
    ui->statuslabel->setText(tr("文件接收被中断！"));
    if(TA_has_Receive == TA_Send_totalBytes)
    {
        TA_isStopped=true;
        ui->statuslabel->setText(tr("文件接收完毕"));
        delete receiver;
    }
}
void TAChatWidget::startSending(const TalkMessage &mes){
    ui->statuslabel->setText(tr("准备发送"));
    initsender();
    TA_time.start();
    sender=new TASendUdpSocket(mes.TA_senderIP,fileName,this);
    connect(sender,SIGNAL(bytesWritten(qint64)),this,SLOT(updateSendProgress(qint64)));
    sender->sendData();
    connect(this,SIGNAL(SendFinished()),this,SLOT(showSendClosedConnect()));
}
