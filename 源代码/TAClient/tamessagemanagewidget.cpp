#include "tamessagemanagewidget.h"

TAMessageManageWidget::TAMessageManageWidget(TAMainWidget *mainWidget,
                                             QWidget *parent) :
    QWidget(parent), m_mainWidget(mainWidget)
{
    if (m_mainWidget == NULL)
    {
        return;
    }
    m_tabWidget= new QTabWidget(this);
    m_localWidget = new TALocalMessageWidget(m_mainWidget, this);
    m_networkWidget = new TANetWorkMessageWidget(m_mainWidget, this);
    setWindowTitle(tr("消息管理"));
    m_tabWidget->addTab(m_localWidget, tr("本地消息"));
    m_tabWidget->addTab(m_networkWidget, tr("网络消息"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_tabWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

//    connect(m_localWidget, SIGNAL(currentIndexClicked(int)),
//            this, SLOT());
//    connect(m_networkWidget, SIGNAL(currentIndexClicked(int)),
//            this, SLOT());
}


TAMessageManageWidget::~TAMessageManageWidget()
{
}


void TAMessageManageWidget::setLocalMessageWidget(const QVector<TalkMessage> & mesVec)
{
    m_localWidget->setMessageShow(mesVec);
}

void TAMessageManageWidget::setNetworkMessageWidget(const QVector<TalkMessage> & mesVec)
{
    m_networkWidget->setMessageShow(mesVec);
}


void TAMessageManageWidget::setListModelGroup()
{
    m_localWidget->setListModelGroup();
    m_networkWidget->setListModelGroup();
}
