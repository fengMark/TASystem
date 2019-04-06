#include "talatestmessagelistwidget.h"

TALatestMessageListWidget::TALatestMessageListWidget(
        TAMainWidget *mainWidget, QWidget *parent) :
    QWidget(parent),  m_layout(new QVBoxLayout), m_mainWidget(mainWidget)
{
    m_mesCounter = 0;
    if (NULL == m_mainWidget)
        return;
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    m_contentsWidget = new QWidget;
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    m_contentsWidget->setLayout(m_layout);
    m_contentsWidget->setStyleSheet("QWidget{border: 0;}");

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setAlignment(Qt::AlignLeft);

    m_scrollArea->setWidget(m_contentsWidget);

    mainLayout->addWidget(m_scrollArea);
    setLayout(mainLayout);

    setStyleSheet("QWidget{border: 0;}");
    creatMenu();
}
void TALatestMessageListWidget::addItem(QWidget *item)
{
    int count = m_layout->count();
    if (count > 1)
    {
        m_layout->removeItem(m_layout->itemAt(count - 1));
    }
    m_layout->addWidget(item);
    m_layout->addStretch();

    emit timerStatus(true);
}
void TALatestMessageListWidget::removeItem(QWidget *item)
{
    m_layout->removeWidget(item);
    if (m_layout->count() == 1)
    {
        m_layout->removeItem(m_layout->itemAt(0));

        emit timerStatus(false);
    }
}
int TALatestMessageListWidget::size()
{
   m_mesCounter = m_layout->count();
   if (m_mesCounter > 1)
       --m_mesCounter;

   return m_mesCounter;
}
void TALatestMessageListWidget::creatMenu()
{
    m_menu = new QMenu(this);
    QAction *showAll = new QAction(tr("查看所有消息"), this);
    QAction *ignoreAll = new QAction(tr("忽略所有消息"), this);

    connect(showAll, SIGNAL(triggered()),
        m_mainWidget, SLOT(showAllChatRoom()));
    connect(ignoreAll, SIGNAL(triggered()),
        m_mainWidget, SLOT(removeAllLinkButton()));

    m_menu->addAction(showAll);
    m_menu->addAction(ignoreAll);
}
void TALatestMessageListWidget::showMenu()
 {
    if (m_menu == NULL)
    {
        creatMenu();
    }
    m_menu->exec(QCursor::pos());
 }
void TALatestMessageListWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        showMenu();
        return;
    }
    QWidget::mousePressEvent(event);
}
