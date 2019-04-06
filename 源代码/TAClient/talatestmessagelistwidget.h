#ifndef TALATESTMESSAGELISTWIDGET_H
#define TALATESTMESSAGELISTWIDGET_H

#include <QWidget>
#include<QScrollArea>
#include<QMenu>
#include<QMouseEvent>
#include<QVBoxLayout>
#include<tamainwidget.h>
class TALatestMessageListWidget : public QWidget
{
    Q_OBJECT
public:
    TALatestMessageListWidget(TAMainWidget *mainWidget, QWidget *parent = 0);
    void addItem(QWidget *item);
    void removeItem(QWidget *item);
    int size();
    //创建右键菜单
    void creatMenu();

signals:
    void timerStatus(bool isOpen);

public slots:
    void showMenu();
protected:
    void mousePressEvent(QMouseEvent *e);

private:
    QVBoxLayout *m_layout;
    QWidget *m_contentsWidget;
    QScrollArea *m_scrollArea;

    //右键菜单
    QMenu *m_menu;

    TAMainWidget *m_mainWidget;

    int m_mesCounter;
};

#endif // TALATESTMESSAGELISTWIDGET_H
