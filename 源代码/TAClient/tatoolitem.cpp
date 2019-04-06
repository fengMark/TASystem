#include "tatoolitem.h"
#include<QDebug>
TAToolItem::TAToolItem(const QString &title, QWidget *parent) :
    QWidget(parent), TA_layout(new QVBoxLayout)
{
    TA_isVisable = false;
    TA_layout = new QVBoxLayout;
    TA_label = new QLabel(title);
    TA_layout->addWidget(TA_label);

    TA_layout->setContentsMargins(0,0,0,0);
    TA_layout->setSpacing(0);
    setLayout(TA_layout);

    creatMenu();
}
void TAToolItem::addItem(QWidget *item)
{
    TA_itemList.append(item);
    TA_layout->addWidget(item);

    if (0 == TA_itemList.size())
        item->setVisible(false);
    else
    {
        item->setVisible(TA_isVisable);
    }
}
void TAToolItem::removeItem(QWidget *item)
{
    TA_layout->removeWidget(item);
    TA_itemList.removeAt(TA_itemList.indexOf(item));
}
void TAToolItem::setTile(const QString & title)
{
    TA_label->setText(title);
}
int TAToolItem::getSize()
{
    return TA_itemList.size();
}
void TAToolItem:: mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        for (int i=0; i<TA_itemList.size(); ++i)
        {
            TA_itemList[i]->setVisible(!TA_itemList[i]->isVisible());
        }

        if (TA_itemList.size() > 0)
            TA_isVisable = TA_itemList[0]->isVisible();
        return;
    }
    else if (event->button() == Qt::RightButton)
    {
        onClickRightButton();
        return;
    }
    QWidget::mousePressEvent(event);
}
void TAToolItem::renameBox()
{
    emit renameBoxSignal(TA_label->text());
}
void TAToolItem::removeBox()
{
    qDebug() << "chose remove";
    emit removeBoxSignal(TA_label->text());
}
void TAToolItem::onClickRightButton()
{
    if (TA_menu == NULL)
    {
        creatMenu();
    }
    TA_menu->exec(QCursor::pos());
}
void TAToolItem:: creatMenu()
{
    TA_menu = new QMenu(this);
    QAction *rename = new QAction(tr("重命名"), TA_menu);
    QAction *remove = new QAction(tr("删除"), TA_menu);
    connect(rename, SIGNAL(triggered()),this, SLOT(renameBox()));
    connect(remove, SIGNAL(triggered()),this, SLOT(removeBox()));
    TA_menu->addAction(rename);
    TA_menu->addAction(remove);
}



