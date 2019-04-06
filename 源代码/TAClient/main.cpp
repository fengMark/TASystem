#include <QtGui/QApplication>
//#include "taloginwidget.h"
#include<QTextCodec>
#include<taloginwidget.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 以下部分解决中文乱码
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    // 以上部分解决中文乱码
    TALoginWidget w;
    w.show();


    return a.exec();
}
