#include <QtGui/QApplication>
#include "taserverwindow.h"
#include "eventdispatcher_epoll.h"
#include<QTextCodec>
int main(int argc, char *argv[])
{
 //   QEventDispatcherEPoll dispatcher;
    // 以下部分解决中文乱码
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    // 以上部分解决中文乱码
    QApplication a(argc, argv);
    TAServerWindow w;
    w.show();
    return a.exec();
}

