#include "taloginwidget.h"
#include "ui_taloginwidget.h"
#include<taclicklabel.h>
#include<QPushButton>
#include<QInputDialog>
#include<model/taencryption.h>
#include<taregisterwidget.h>
#include<model/tatcpsocket.h>
#include<tamainwidget.h>
#include<control/taclientfilectl.h>
#include<model/taencryption.h>
#include<control/tamainctl.h>
#include<QLineEdit>
#include<QHBoxLayout>
#include<QHostAddress>
#include<control/taloginctl.h>
#include<QLabel>
#include<QDebug>
static const QString AUTO_LOGIN_FILE_NAME = "/home/yas/commuciate/TAClient/resource/config/auto_login.im";
TALoginWidget::TALoginWidget(QWidget *parent) :QWidget(parent),ui(new Ui::TALoginWidget)
{
    ui->setupUi(this);
    QFile file(AUTO_LOGIN_FILE_NAME);

    if(file.exists()){
        qDebug()<<"exist";
        QString uid;
        QString password;
        bool ischecked=false;
        TAClientFileCtl::readAutoLogin(AUTO_LOGIN_FILE_NAME,uid,password,ischecked);
        if(ischecked==true){
            ui->UID_lineEdit->setText(uid);
            ui->PWDlineEdit->setText(password);
            ui->memButton->setChecked(ischecked);
        }
    }
    initLoginwidget();
    setWindowFlags(this->windowFlags()&(~(Qt::WindowMinMaxButtonsHint|Qt::WindowMinimizeButtonHint)));
    setWindowTitle(tr("登录TA"));
    linkSignalAndSlot();
}
TALoginWidget::~TALoginWidget()
{
    delete ui;
    if(TA_loginCtl != NULL){
        delete TA_loginCtl;
        TA_loginCtl=NULL;
    }
}
void TALoginWidget::setServer(const QString &ip, const int port){//设置TCP和UDP通信的IP和端口
    QHostAddress serverAddress;
    int serverPort;
    if (TA_hostAddress->text().isEmpty())
    {
        serverAddress = ip;
    }
    else
    {
        serverAddress = TA_hostAddress->text();
    }
    if (TA_hostPort->text().isEmpty())
    {
        serverPort = port;
    }
    else
    {
        serverPort = TA_hostPort->text().toInt();
    }
    TATcpSocket::s_hostAddress = serverAddress;
    TATcpSocket::s_hostPort = serverPort;
}
void TALoginWidget::timerEvent(QTimerEvent *event){
    if (event->timerId() == TA_timerID)
    {
        ++TA_counter;
        switch (TA_counter%4)
        {
        case 0:
            TA_labelStatus->setText(tr("登录中"));
            break;
        case 1:
            TA_labelStatus->setText(tr("登录中.."));
            break;
        case 2:
            TA_labelStatus->setText(tr("登录中....."));
            break;
        case 3:
            TA_labelStatus->setText(tr("登录中......."));
            break;
        }
    }
}
void TALoginWidget::setLabelStatus(const QString &mes, bool isLogin, const UserInformation *me){
    closeTimer();
    if (isLogin == true)
    {
        ui->loginButton->setEnabled(false);
        ui->loginButton->setText(tr("取消"));
        TA_labelStatus->setText(tr("%1").arg(mes));
        if (true == ui->memButton->isChecked())//记住密码写登录文件
        {
            if (!TAClientFileCtl::writeAutoLogin(AUTO_LOGIN_FILE_NAME,ui->UID_lineEdit->text(), ui->PWDlineEdit->text(),ui->memButton->isChecked()))
            {
                qDebug() << "fail to write autoLogin file.";
            }
        }
        else
        {
            if (!TAClientFileCtl::remove(AUTO_LOGIN_FILE_NAME))
            {
                qDebug() << "fail to remove autoLogin file.";
            }
        }

        TAMainWidget *mainWidget = new TAMainWidget(*me);

        mainWidget->getFriendsInformation();
        mainWidget->show();

        close();
    }
    else
    {
        isLogin=true;
        ui->loginButton->setText(tr("登录"));
        this->TA_labelStatus->setText(mes);
        ui->UID_lineEdit->setReadOnly(false);
        ui->PWDlineEdit->setReadOnly(false);
    }
}
void TALoginWidget::initLoginwidget(){
    isLogin=true;
    TA_loginCtl = new TALoginCtl(this);
    connect(TA_loginCtl, SIGNAL(getLoginMessgae(QString,bool,const UserInformation*)),this, SLOT(setLabelStatus(QString,bool,const UserInformation*)));
    TA_counter=0;
    TA_closeTimer=true;
    TA_timerID=0;
    qsrand(time(NULL));
    ui->labelheadline->setText(tr("登录即时通信系统"));
    ui->labelheadline->setAlignment(Qt::AlignCenter);
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::black);
    ui->labelheadline->setPalette(pa);
    QFont ft;
    ft.setPointSize(15);
    ui->labelheadline->setFont(ft);
    ui->PWDlineEdit->setEchoMode(QLineEdit::Password);
    TARegisterLabel=new TAClickLabel;
    TARegisterLabel->setText(tr("注册帐号"));
    ui->horizontalLayout_1->addWidget(TARegisterLabel);
    TAForgetPwdLabel=new TAClickLabel;
    TAForgetPwdLabel->setText(tr("忘记密码"));
    ui->horizontalLayout_2->addWidget(TAForgetPwdLabel);

    ui->moreButton->setCheckable(true);
    ui->moreButton->setAutoDefault(false);


    TA_hostAddress = new QLineEdit;
    TA_hostAddress->setPlaceholderText(tr("服务器IP地址"));
    TA_hostPort = new QLineEdit;
    TA_hostPort->setFixedWidth(60);
    TA_hostPort->setPlaceholderText(tr("端口号"));
    TA_extendedWidget = new QWidget;
    hLayoutExtended = new QHBoxLayout;
    hLayoutExtended->setMargin(0);
    hLayoutExtended->addWidget(TA_hostAddress);
    hLayoutExtended->addWidget(TA_hostPort);
    TA_extendedWidget->setLayout(hLayoutExtended);

    TA_labelStatus = new QLabel;
    TA_labelStatus->setText(tr("你好,请登录系统."));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    TA_labelStatus->setSizePolicy(sizePolicy);
    ui->verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
    ui->verticalLayout->addWidget(TA_extendedWidget);
    ui->verticalLayout->addWidget(TA_labelStatus);

    TA_extendedWidget->hide();


}
void TALoginWidget::linkSignalAndSlot(){
    connect(ui->moreButton, SIGNAL(toggled(bool)),TA_extendedWidget, SLOT(setVisible(bool)));
    connect(ui->loginButton, SIGNAL(clicked()),this, SLOT(onClickButtonLogin()));
    connect(TARegisterLabel, SIGNAL(clicked()),this, SLOT(onClickLabelRegister()));
    connect(TAForgetPwdLabel, SIGNAL(clicked()),this, SLOT(onClickLabelForgot()));
}
void TALoginWidget::onClickButtonLogin(){
    setServer();

    ui->UID_lineEdit->setReadOnly(true);
    ui->PWDlineEdit->setReadOnly(true);

    if (true == isLogin)
    {
        isLogin = false;
        ui->loginButton->setText(tr("取消"));
        TA_closeTimer=false;
        TA_timerID=startTimer(1000);//interval is 100ms
        TA_labelStatus->setText(tr("登录中"));
        if (TA_loginCtl == NULL)
        {
            TA_loginCtl = new TALoginCtl(this);
            connect(TA_loginCtl, SIGNAL(getLoginMessgae(QString,bool,const UserInformation*)),this, SLOT(setLabelStatus(QString,bool,const UserInformation*)));
        }
        switch (ui->comboBox->currentIndex())
        {
        case 0:
            TA_status = ONLINE;
            break;
        case 1:
            TA_status = INVISIBLE;
            break;
        case 2:
            TA_status = BUSY;
            break;
        case 3:
            TA_status = LEAVE;
            break;
        case 4:
            TA_status = NOTDISTURB;
            break;
        default:
            TA_status = ONLINE;
            break;
        }
        QString pwd = TAEncryption::getInstace().getXorEncryptDecrypt(ui->PWDlineEdit->text(), 10);
        TA_loginCtl->login(ui->UID_lineEdit->text(), pwd, TA_status);
    }
    else
    {
        isLogin = true;
        ui->loginButton->setText(tr("登录"));
        TA_labelStatus->setText(tr("你好,请登录系统."));
        if (TA_loginCtl != NULL)
        {
            delete TA_loginCtl;
            TA_loginCtl = NULL;
        }
        closeTimer();
    }
}
void TALoginWidget::closeTimer(){
    if (TA_timerID != 0)
    {
        TA_closeTimer = true;
        killTimer(TA_timerID);//kill timer
        TA_timerID = 0;
        TA_counter = 0;
    }
}
void TALoginWidget::onClickLabelForgot(){
    setServer();
    bool isOkMes = false;
    QString userID = QInputDialog::getText(NULL, "找回密码", "请输入帐号ID",QLineEdit::Normal,NULL,&isOkMes);
    if (!isOkMes)
        return;
    TA_loginCtl->getQuestionAndAnswer(userID);
}
void TALoginWidget::onClickLabelRegister(){
    setServer();
    TARegisterWidget *registerWidget = new TARegisterWidget;
    registerWidget->show();
}

