#ifndef TACLICKLABEL_H
#define TACLICKLABEL_H

#include <QLabel>
#include<QMouseEvent>
#include<QEvent>
class TAClickLabel : public QLabel
{
    Q_OBJECT
public:
    explicit TAClickLabel(QWidget *parent = 0);
    TAClickLabel(const QString &text, QWidget *parent=0);
signals:
    void clicked();
protected:
    void mouseReleaseEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
private:
    int MyLabelPressed;
};

#endif // TACLICKLABEL_H
