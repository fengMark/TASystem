#ifndef TAREIVERUDPSOCKEET_H
#define TAREIVERUDPSOCKEET_H

#include <QUdpSocket>
#include<QFile>
class TAReiverUdpSockeet : public QUdpSocket
{
    Q_OBJECT
public:
    explicit TAReiverUdpSockeet(QString &filename,QObject *parent = 0);
    ~TAReiverUdpSockeet();
signals:
    void byread(qint64);
public slots:
    void readPendingDatagrams();
private:
    void initSocket();

QFile file;
QString filename;
};

#endif // TAREIVERUDPSOCKEET_H
