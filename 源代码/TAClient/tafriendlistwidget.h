#ifndef TAFRIENDLISTWIDGET_H
#define TAFRIENDLISTWIDGET_H

#include <QWidget>

class QVBoxLayout;
class QScrollArea;
class TAMainWidget;
class QMouseEvent;
class QMenu;
class TAFriendListWidget : public QWidget
{
    Q_OBJECT
public:
    TAFriendListWidget(TAMainWidget *mainWidget, QWidget *parent = 0);
    void addItem(QWidget *item);

    //创建右键菜单
    void creatMenu();

signals:


public slots:
    void showMenu();

protected:
    void mousePressEvent(QMouseEvent *e);

private:
    QVBoxLayout *m_layout;
    QWidget *m_contentsWidget;
    QScrollArea *m_friendsScrollArea;

    //右键菜单
    QMenu *m_menu;

    TAMainWidget *m_mainWidget;

};

#endif // TAFRIENDLISTWIDGET_H
