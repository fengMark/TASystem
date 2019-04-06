#ifndef TATHREAD_H
#define TATHREAD_H

#include <QThread>
#include<control/taclientsocketctl.h>

class TAHandleMessage;
class TAThread : public QThread
{
    Q_OBJECT
public:
    explicit TAThread(QObject *parent = 0);
    explicit TAThread( QObject * parent,int socketDescriptor);
    ~TAThread();

signals:

public slots:

private:
    void run(); // 线程运行时，会调用此函数

private:
    int m_socketDescriptor;
    TAClientSocketCtl * m_clientSocket;
    TAHandleMessage *m_handleMes;
};

#endif // TATHREAD_H
