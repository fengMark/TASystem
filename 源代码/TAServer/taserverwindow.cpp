#include "taserverwindow.h"
#include "ui_taserverwindow.h"

TAServerWindow::TAServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TAServerWindow)
{
    ui->setupUi(this);
    TA_serverCtrl = new TAServerMessageCtl(this);
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(onClickStartButton()));
    connect(ui->StopButton,SIGNAL(clicked()),this,SLOT(onClickStopButton()));
    setWindowTitle(tr("TA Server"));
}
void TAServerWindow::onClickStartButton(){
    QHostAddress serverAddress;
    int serverPort;
    if (ui->ipAddress->text().isEmpty())
    {
        ui->label->setText(tr("IP地址不能为空哦"));
        return;
    }
    else
    {
        serverAddress = ui->ipAddress->text();
    }
    if (ui->port->text().isEmpty())
    {
        ui->label->setText(tr("IP地址不能为空哦"));
        return;
    }
    else
    {
        serverPort = ui->port->text().toInt();
    }
    TAServerMessageCtl::s_hostAddress=serverAddress;
    TAServerMessageCtl::s_hostPort=serverPort;
    ui->label->setText(tr("启动服务"));
     TA_serverCtrl->runServer();
}
void TAServerWindow::onClickStopButton(){
    ui->label->setText(tr("关闭服务"));
     TA_serverCtrl->stopServer();
}
TAServerWindow::~TAServerWindow()
{
    delete ui;
    if(TA_serverCtrl!=NULL){
        delete  TA_serverCtrl;
        TA_serverCtrl=NULL;
    }
}
