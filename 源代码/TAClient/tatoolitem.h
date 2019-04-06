#ifndef TATOOLITEM_H
#define TATOOLITEM_H

#include <QObject>
#include<QMouseEvent>
#include<QLabel>
#include<QList>
#include<QMenu>
#include<QVBoxLayout>
class TAToolItem : public QWidget
{
    Q_OBJECT
public:
    TAToolItem(const QString &title, QWidget *parent = 0);

    void addItem(QWidget *item);

    void removeItem(QWidget *item);

    void setTile(const QString &);

    int getSize();

    QList<QWidget *> * getWidgetList()
    {
        return &TA_itemList;
    }

signals:
    void renameBoxSignal(const QString & title);
    void removeBoxSignal(const QString & title);

protected:
    void mousePressEvent(QMouseEvent *event);

private slots:
    void renameBox();
    void removeBox();
    void onClickRightButton();

private:
    void creatMenu();

private:
    QList<QWidget *> TA_itemList;
    QVBoxLayout *TA_layout;
    bool TA_isVisable;
    QMenu *TA_menu;
    QLabel *TA_label;

};

#endif // TATOOLITEM_H
