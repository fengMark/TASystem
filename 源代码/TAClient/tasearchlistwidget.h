#ifndef TASEARCHLISTWIDGET_H
#define TASEARCHLISTWIDGET_H

#include <QWidget>
#include<QVBoxLayout>
#include<QScrollArea>
#include<tamainwidget.h>
class TASearchListWidget : public QWidget
{
    Q_OBJECT
public:
    TASearchListWidget(QWidget *parent = 0);
    void addItem(QWidget *item);
    void removeItem(QWidget *item);
    int size();
    void clearAll();

signals:

public slots:

private:
    QVBoxLayout *m_layout;
    QWidget *m_contentsWidget;
    QScrollArea *m_scrollArea;
    TAMainWidget *m_mainWidget;

    int m_mesCounter;
};

#endif // TASEARCHLISTWIDGET_H
