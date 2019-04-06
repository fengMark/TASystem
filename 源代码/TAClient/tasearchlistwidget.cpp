#include "tasearchlistwidget.h"

TASearchListWidget::TASearchListWidget(QWidget *parent) :
    QWidget(parent),  m_layout(new QVBoxLayout)
{
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
}


/*************************************************
Function Name： addItem()
Description:  添加
*************************************************/
void TASearchListWidget::addItem(QWidget *item)
{
    int count = m_layout->count();

    if (count > 1)
    {
        m_layout->removeItem(m_layout->itemAt(count - 1));
    }
    m_layout->addWidget(item);
    m_layout->addStretch();
}

/*************************************************
Function Name： removeItem()
Description:  删除
*************************************************/
void TASearchListWidget::removeItem(QWidget *item)
{
    m_layout->removeWidget(item);
    if (m_layout->count() == 1)
    {
        m_layout->removeItem(m_layout->itemAt(0));
    }
}

/*************************************************
Function Name： clearAll()
Description:  删除所有
*************************************************/
void TASearchListWidget::clearAll()
{
    QLayoutItem *child;
     while ((child = m_layout->takeAt(0)) != 0)
     {
         m_layout->removeItem(child);
         QWidget *widget = child->widget();
         if (widget != NULL)
         widget->deleteLater();
     }
}

/*************************************************
Function Name： size()
Description:  size
*************************************************/
int TASearchListWidget::size()
{
    m_mesCounter = m_layout->count();
    if (m_mesCounter > 1)
        --m_mesCounter;

    return m_mesCounter;
}
