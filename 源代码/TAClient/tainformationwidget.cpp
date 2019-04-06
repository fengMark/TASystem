#include "tainformationwidget.h"
#include "ui_tainformationwidget.h"
#include<QFileInfo>
#include<tamainwidget.h>
#include<taclicklabel.h>
#include<QHBoxLayout>
#include<QTcpServer>
#include<QTcpSocket>
#include<QFileDialog>
TAInformationWidget::TAInformationWidget(TAMainWidget *mainWidget,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TAInformationWidget)
{
    ui->setupUi(this);
    m_mainWidget=mainWidget;
    headlabel=new TAClickLabel(tr("头像"),this);
    m_isReadOnly = true;
    m_isEdit = false;
    setWindowTitle(tr("个人信息"));
    setWindowFlags(this->windowFlags()&(~(Qt::WindowMinMaxButtonsHint|Qt::WindowMinimizeButtonHint)));
    linkSignalWithSlot();
}

TAInformationWidget::~TAInformationWidget()
{
    delete ui;
}
void TAInformationWidget::setReadOnly(bool isReadOnly, bool isEdit){
    m_isReadOnly = isReadOnly;
    m_isEdit = isEdit;
    ui->UserID->setReadOnly(true);
    ui->NickName->setReadOnly(isReadOnly);
    ui->SexcomboBox->setEnabled(!isReadOnly);
    ui->Phone->setReadOnly(isReadOnly);
    ui->editButton->setVisible(isEdit);

    if (isEdit)
    {
        if (isReadOnly)
        {
            ui->CancelButton->setText(tr("关闭"));
            ui->editButton->setText(tr("编辑"));
        }
        else
        {
            ui->CancelButton->setText(tr("取消"));
            ui->editButton->setText(tr("保存"));
        }
    }
    else
    {
        ui->CancelButton->setText(tr("关闭"));
    }
}
void TAInformationWidget::setUserInformation(const UserInformation &user){
    m_userInf = user;
    ui->UserID->setText(m_userInf.TA_userID);
    ui->NickName->setText(m_userInf.TA_nickname);
    if (0 == m_userInf.TA_sex.compare("男"))
        ui->SexcomboBox->setCurrentIndex(0);
    else
        ui->SexcomboBox->setCurrentIndex(1);
    ui->Phone->setText(m_userInf.TA_phone);

    m_headNum = m_userInf.TA_headPortrait;
    QString str = QString(":/image/resource/image/head/%1.bmp").arg(QString::number(m_headNum));
    headlabel->setPixmap(QPixmap(str));
    headlabel->setFixedSize(HEAD_BIG_SIZE,HEAD_BIG_SIZE);
    QHBoxLayout *headlayout=new QHBoxLayout;
    headlayout->addWidget(headlabel);
    ui->widget->setLayout(headlayout);
}
void TAInformationWidget::linkSignalWithSlot(){
connect(headlabel,SIGNAL(clicked()),this,SLOT(onClickHeadLabel()));
connect(ui->editButton,SIGNAL(clicked()),this,SLOT(onClickOKButton()));
connect(ui->CancelButton,SIGNAL(clicked()),this,SLOT(onClickCancelButton()));
}
void TAInformationWidget::onClickOKButton(){
    if (!m_isReadOnly)
    {
        m_userInf=getInformation();
        m_mainWidget->changeMyInformation(m_userInf);
    }
    setReadOnly(!m_isReadOnly, true);
}
void TAInformationWidget::sendData(){
    qint64 len = 0;
        do{
            len = 0;
            char buf[4*1024] = {0};//每次发送数据大小
            len = file.read(buf,sizeof(buf));//读文件
            len = socket->write(buf,len);//发文件
            sendSize += len;
        }while(len > 0);

        if(sendSize != fileSize){
            file.close();
            return ;
        }
        file.close();
}
void TAInformationWidget::sendIconFile(int TA_headProait){
    QString toDir=QString("/home/yas/commuciate/TAClient/resource/image/head/1%.bmp").arg(QString::number(TA_headProait));
    QFile::copy(icon_name, toDir);

        fileName.clear();
        fileSize = 0;
        sendSize = 0;
        QFileInfo info(toDir);
        fileName = info.fileName();
        fileSize = info.size();
        file.setFileName(toDir);
        if(!file.open(QIODevice::ReadOnly)){
            return;
        }
    file.rename(icon_name,QString::number(TA_headProait));
    server = new QTcpServer(this);
    server->listen(QHostAddress::Any,6666);
    connect(server,SIGNAL(newConnection()),this,SLOT(sendData()));
    socket=server->nextPendingConnection();
}
void TAInformationWidget::onClickHeadLabel(){

    QFileDialog *fileDialog = new QFileDialog(this);//创建一个QFileDialog对象，构造函数中的参数可以有所添加。
    fileDialog->setWindowTitle(tr("Select Icon"));//设置文件保存对话框的标题
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);//设置文件对话框为保存模式
    fileDialog->setFileMode(QFileDialog::AnyFile);//设置文件对话框弹出的时候显示任何文件，不论是文件夹还是文件
    fileDialog->setViewMode(QFileDialog::Detail);//文件以详细的形式显示，显示文件名，大小，创建日期等信息；
    fileDialog->setGeometry(10,30,300,200);//设置文件对话框的显示位置
    fileDialog->setDirectory(".");//设置文件对话框打开时初始打开的位置
    fileDialog->setFilter(tr("Image Files(*.jpg *.png)"));//设置文件类型过滤器
    if(fileDialog->exec() == QDialog::Accepted)
    {
        icon_name=QString(fileDialog->selectedFiles()[0]);//得到用户选择的文件名
        headlabel->setPixmap(QPixmap(icon_name).scaled(40,40,Qt::KeepAspectRatio));//显示选择的图片/*

        m_mainWidget->uploadIconFile(icon_name);//头像文件上传服务器
    }


}
void TAInformationWidget::onClickCancelButton(){
    if (!m_isReadOnly)
    {
        setUserInformation(m_userInf);
        setReadOnly(!m_isReadOnly, m_isEdit);
    }
    else
        close();
}
UserInformation TAInformationWidget::getInformation(){
    m_userInf.TA_nickname = ui->NickName->text();
    m_userInf.TA_sex = ui->SexcomboBox->currentText();
    m_userInf.TA_phone = ui->Phone->text();
    m_userInf.TA_userID=ui->UserID->text();
    m_userInf.TA_headPortrait = m_headNum;
    return m_userInf;
}
void TAInformationWidget::closeEvent(QCloseEvent *)
{
if (NULL != m_mainWidget)
    m_mainWidget->removeInformationWidget(m_userInf.TA_userID);
}
