#include "taclicklabel.h"

TAClickLabel::TAClickLabel(QWidget *parent) :
    QLabel(parent)
{
    MyLabelPressed=0;
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::darkGreen);
    setPalette(pa);
}
TAClickLabel::TAClickLabel(const QString &text, QWidget *parent) :
    QLabel(parent)
{
    MyLabelPressed=0;
    setText(text);
}
void TAClickLabel::enterEvent(QEvent * )
{
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::blue);
    setPalette(pa);
}

void TAClickLabel::leaveEvent(QEvent *)
{
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::darkGreen);
    setPalette(pa);
}
void TAClickLabel::mousePressEvent(QMouseEvent *ev){
    if(ev->button() == Qt::LeftButton)
        MyLabelPressed = 1;
}
void TAClickLabel::mouseReleaseEvent(QMouseEvent *ev){
    if ((ev->button() == Qt::LeftButton)&&(MyLabelPressed==1)){
        emit clicked();
        MyLabelPressed = 0;
    }
}
