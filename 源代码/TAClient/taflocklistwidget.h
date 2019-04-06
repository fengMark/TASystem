#ifndef TAFLOCKLISTWIDGET_H
#define TAFLOCKLISTWIDGET_H

#include <QWidget>
#include<QScrollArea>
#include<QMenu>
#include<tamainwidget.h>
#include<QMouseEvent>
#include<QVBoxLayout>
class TAFlockListWidget : public QWidget
{
    Q_OBJECT
public:
    TAFlockListWidget(TAMainWidget *mainWidget, QWidget *parent = 0);
    void addItem(QWidget *item);
    void removeItem(QWidget *item);
    void creatMenu();

signals:

public slots:
    void showMenu();

protected:
    void mousePressEvent(QMouseEvent *e);

private:
    QVBoxLayout *m_layout;
    QWidget *m_contentsWidget;
    QScrollArea *m_flocksScrollArea;
    QMenu *m_menu;

    TAMainWidget *m_mainWidget;
};

#endif // TAFLOCKLISTWIDGET_H
