#ifndef TAFLOCKMEMBERLISTWIDGET_H
#define TAFLOCKMEMBERLISTWIDGET_H

#include <QWidget>
class QVBoxLayout;
class QScrollArea;
class TAFlockMemberListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TAFlockMemberListWidget(QWidget *parent = 0);
    void addItem(QWidget *item);

signals:

public slots:

private:
    QVBoxLayout *m_layout;
    QWidget *m_contentsWidget;
    QScrollArea *m_flocksScrollArea;
};

#endif // TAFLOCKMEMBERLISTWIDGET_H
