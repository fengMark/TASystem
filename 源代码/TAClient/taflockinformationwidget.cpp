#include "taflockinformationwidget.h"
#include "ui_taflockinformationwidget.h"
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>
#include<tamainwidget.h>
TAFlockInformationWidget::TAFlockInformationWidget(TAMainWidget *mainWidget,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TAFlockInformationWidget)
{
    ui->setupUi(this);
    m_mainWidget = mainWidget;
    initTALoginWidget();
    setWindowTitle(tr("群信息"));
    setWindowFlags(this->windowFlags()&(~(Qt::WindowMinMaxButtonsHint|Qt::WindowMinimizeButtonHint)));
    setReadOnly(true, false);

    connect(ui->CancelButton, SIGNAL(clicked()),this, SLOT(onClickCancelButton()));
    connect(ui->OKButton, SIGNAL(clicked()),this, SLOT(onClickOKButton()));
}
TAFlockInformationWidget::~TAFlockInformationWidget()
{
    delete ui;
}
void TAFlockInformationWidget::initTALoginWidget(){
    m_isReadOnly = true;
    m_isEdit = false;

    QString str = QString(":/image/resource/image/flock.png");
    QPixmap pixmap;
    QIcon icon(str);
    pixmap = icon.pixmap(QSize(HEAD_BIG_SIZE, HEAD_BIG_SIZE));
    ui->headlabel->setPixmap(pixmap);
}
void TAFlockInformationWidget::closeEvent(QCloseEvent *){
    if (NULL != m_mainWidget)
        m_mainWidget->removeFlockInformationWidget(m_flockInf.TA_flockID);
}
void TAFlockInformationWidget::onClickCancelButton(){
    if (!m_isReadOnly)
    {
        setFlockInformation(m_flockInf);
        setReadOnly(!m_isReadOnly, m_isEdit);
    }
    else
        close();
}
void TAFlockInformationWidget::onClickOKButton(){
    if (!m_isReadOnly)
    {
        getInformation();
        m_mainWidget->changeFlockInformation(m_flockInf);
    }
    setReadOnly(!m_isReadOnly, true);
}
FlockInformation TAFlockInformationWidget::getInformation(){
    m_flockInf.TA_flockName = ui->flockNickname->text();
    m_flockInf.TA_theme = ui->flockTheme->text();
    return m_flockInf;
}
void TAFlockInformationWidget::setFlockInformation(const FlockInformation &flock){
    m_flockInf = flock;
    ui->flockID->setText(m_flockInf.TA_flockID);
    ui->flockNickname->setText(m_flockInf.TA_flockName);
    ui->flockTheme->setText(m_flockInf.TA_theme);
    ui->CreatorID->setText(m_flockInf.TA_creatorID);
    ui->CreatorDate->setText(m_flockInf.TA_createDate.toString("yyyy-MM-dd"));
}
void TAFlockInformationWidget::setReadOnly(bool isReadOnly, bool isEdit){
    m_isReadOnly = isReadOnly;
    m_isEdit = isEdit;

    ui->flockID->setReadOnly(true);
    ui->CreatorID->setReadOnly(true);
    ui->CreatorDate->setReadOnly(true);

    ui->flockNickname->setReadOnly(isReadOnly);
    ui->flockTheme->setReadOnly(isReadOnly);

    ui->OKButton->setVisible(isEdit);

    if (isEdit)
    {
        if (isReadOnly)
        {
            ui->CancelButton->setText(tr("关闭"));
            ui->OKButton->setText(tr("编辑"));
        }
        else
        {
            ui->CancelButton->setText(tr("取消"));
            ui->OKButton->setText(tr("保存"));
        }
    }
    else
    {
        ui->CancelButton->setText(tr("关闭"));
    }
}
