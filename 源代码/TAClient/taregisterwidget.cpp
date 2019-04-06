#include "taregisterwidget.h"
#include "ui_taregisterwidget.h"
#include<QMessageBox>
#include<model/taencryption.h>
#include<QFont>
TARegisterWidget::TARegisterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TARegisterWidget)
{
    ui->setupUi(this);
    initRegisterWidget();
    setWindowTitle(tr("注册"));
    linkSignalWithSlot();
    setWindowFlags(this->windowFlags()&(~(Qt::WindowMinMaxButtonsHint|Qt::WindowMinimizeButtonHint)));
}

TARegisterWidget::~TARegisterWidget()
{
    delete ui;

}
void TARegisterWidget::initRegisterWidget(){
    ui->TA_headlabel->setText(tr("注册信息"));
    ui->TA_headlabel->setAlignment(Qt::AlignCenter);
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::black);
    ui->TA_headlabel->setPalette(pa);
    QFont ft;
    ft.setPointSize(15);
    ui->TA_headlabel->setFont(ft);

    ui->password_lineEdit->setEchoMode(QLineEdit::Password);
    ui->repatepwd_lineEdit->setEchoMode(QLineEdit::Password);
}
void TARegisterWidget::linkSignalWithSlot(){
    connect(ui->RegisterButton,SIGNAL(clicked()),this,SLOT(onClickBtnRegister()));
    connect(ui->CancelButton,SIGNAL(clicked()),this,SLOT(onClickBtnCancel()));
}
void TARegisterWidget::onClickBtnCancel(){
    close();
}
void TARegisterWidget::onClickBtnRegister(){
    if (ui->nick_lineEdit->text().isEmpty())
    {
        QMessageBox::about(NULL, tr("提示"), tr("昵称不能为空"));
        return;
    }
    if(ui->phone_lineEdit->text().isEmpty()){
        QMessageBox::about(NULL, tr("提示"), tr("手机号不能为空"));
        return;
    }
    if(ui->password_lineEdit->text().isEmpty()){
        QMessageBox::about(NULL, tr("提示"), tr("密码不能为空"));
        return;
    }
    if(ui->repatepwd_lineEdit->text().isEmpty()){
        QMessageBox::about(NULL, tr("提示"), tr("密码不能为空"));
        return;
    }
    // 判断密码是否一致
    if (ui->repatepwd_lineEdit->text().compare(ui->password_lineEdit->text()) != 0)
    {
        QMessageBox::about(NULL, tr("提示"), tr("密码不一致"));
        return;
    }
    if(ui->question_lineEdit->text().isEmpty()){
        QMessageBox::about(NULL, tr("提示"), tr("密保问题不能为空"));
        return;
    }
    if(ui->answer_lineEdit->text().isEmpty()){
        QMessageBox::about(NULL, tr("提示"), tr("密保答案不能为空"));
        return;
    }
    TA_info.TA_nickname =  ui->nick_lineEdit->text();
    TA_info.TA_password = TAEncryption::getInstace().getXorEncryptDecrypt(ui->password_lineEdit->text(), 10);
    TA_info.TA_sex = ui->comboBox->currentText();
    TA_info.TA_phone = ui->phone_lineEdit->text();
    TA_info.TA_question = ui->question_lineEdit->text();
    TA_info.TA_answer = ui->answer_lineEdit->text();
    TA_info.TA_regDateTime = QDateTime::currentDateTime();


    if ( 0 == TA_info.TA_sex.compare("男"))
        TA_info.TA_headPortrait = 43;
    else
        TA_info.TA_headPortrait = 20;

    if (TA_RegCtl == NULL)
    {
        TA_RegCtl = new TARegisterCtl;
        connect(TA_RegCtl, SIGNAL(signalResetBtnRegister(bool )),this, SLOT(resetBtnRegister(bool )));
    }
    ui->RegisterButton->setEnabled(false);
    TA_RegCtl->registerAccount(TA_info);
}
void TARegisterWidget::resetBtnRegister(bool enable){
    ui->RegisterButton->setEnabled(enable);
}
