#ifndef TAENCRYPTION_H
#define TAENCRYPTION_H

#include <QObject>
#include<QMutex>
#include<QReadWriteLock>
#include<QAtomicPointer>
#include<QTextCodec>
class TAEncryption
{
public:
    static TAEncryption& getInstace(void);
    QString getXorEncryptDecrypt(const QString &, const char &);
    QString byteToQString(const QByteArray &byte);
    QByteArray qstringToByte(const QString &strInfo);
private:
    TAEncryption(){}//禁止构造函数。
    TAEncryption(const  TAEncryption &);//禁止拷贝构造函数。
    TAEncryption & operator =(const  TAEncryption &);//禁止赋值拷贝函数。

    QReadWriteLock m_internalMutex;//函数使用的读写锁。
    static  QMutex s_mutex;//实例互斥锁。
    static  QAtomicPointer<TAEncryption> s_instance; //使用原子指针,默认初始化为0

};

#endif // TAENCRYPTION_H
