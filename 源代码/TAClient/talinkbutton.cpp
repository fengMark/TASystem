#include "talinkbutton.h"
#include <QBoxLayout>


TALinkButton::TALinkButton(QWidget *parent) :
    QPushButton(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    m_labelHead = new QLabel;
    m_labelHead->setFixedSize(HEAD_SMALL_SIZE, HEAD_SMALL_SIZE);
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->setContentsMargins(0,0,0,0);
    m_labelInfo = new QLabel;
    m_labelMes = new QLabel;
    rightLayout->addWidget(m_labelInfo);
    rightLayout->addWidget(m_labelMes);

    mainLayout->addWidget(m_labelHead);
    mainLayout->addLayout(rightLayout);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
    setMinimumSize(100,40);

    m_menu = NULL;

    setStyleSheet("QPushButton{border: 0;}");
}
void TALinkButton::setLatestMessage(const TalkMessage & mes)
{
    m_labelMes->setText(mes.TA_text);
}
void TALinkButton::onClickRightButton()
{
    if (m_menu == NULL)
    {
        creatMenu();
    }
    m_menu->exec(QCursor::pos());
}
void TALinkButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        onClickRightButton();
        return;
    }
    QPushButton::mousePressEvent(event);
}
void TALinkButton::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        onDoubleClickButton();
        return;
    }
    QPushButton::mouseDoubleClickEvent(event);
}
void TALinkButton::creatMenu()
{
    m_menu = new QMenu(this);
    QAction *showMessage = new QAction(tr("查看消息"), m_menu);
    QAction *ignoreMessage = new QAction(tr("忽略消息"), m_menu);
    connect(showMessage, SIGNAL(triggered()),
            this, SLOT(onDoubleClickButton()));
    connect(ignoreMessage, SIGNAL(triggered()),
            this, SLOT(ignoreMessage()));

    m_menu->addAction(showMessage);
    m_menu->addAction(ignoreMessage);
}
