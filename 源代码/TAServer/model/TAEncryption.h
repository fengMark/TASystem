#ifndef TAENCRYPTION_H
#define TAENCRYPTION_H
#include <QAtomicPointer>
#include <QMutexLocker>
#include <QReadWriteLock>
#include <QString>
#include <QDebug>
#include <QTextCodec>

class TAEncryption
{
public:
    // 用户获取实例，返回实例的引用
    static TAEncryption& getInstace(void);

    // 使用异或运算加密
    QString getXorEncryptDecrypt(const QString &, const char &);
    // QByteArray 转 QString
    QString byteToQString(const QByteArray &byte);
    // QString 转 QByteArray
    QByteArray qstringToByte(const QString &strInfo);

private:
    TAEncryption(){}//禁止构造函数。
    TAEncryption(const  TAEncryption &);//禁止拷贝构造函数。
    TAEncryption & operator =(const  TAEncryption &);//禁止赋值拷贝函数。

    QReadWriteLock m_internalMutex;//函数使用的读写锁。
    static  QMutex s_mutex;//实例互斥锁。
    static  QAtomicPointer<TAEncryption> s_instance; //使用原子指针,默认初始化为0
};

QMutex TAEncryption::s_mutex;//实例互斥锁。
QAtomicPointer<TAEncryption> TAEncryption::s_instance = 0; //使用原子指针,默认初始化为0

TAEncryption& TAEncryption::getInstace(void)
{
    #ifndef Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
    // 运行时检测
    if (!QAtomicPointer::isTestAndSetNative())
    {
        qDebug() << "Error: TestAndSetNative not supported!";
    }
    #endif

    //使用双重检测。
    //testAndSetOrders操作保证在原子操作前和后的的内存访问不会被重新排序。

    if (s_instance.testAndSetOrdered(0, 0))//第一次检测
    {
        QMutexLocker locker(&s_mutex);//加互斥锁。
        s_instance.testAndSetOrdered(0, new  TAEncryption);//第二次检测。
    }
    return  *s_instance;
}

QString TAEncryption::getXorEncryptDecrypt(const QString &str, const char &key)
{
    QString result;
    QByteArray bs = qstringToByte(str);

    for(int i=0; i<bs.size(); i++)
    {
        bs[i] = bs[i] ^ key;
    }

    result = byteToQString(bs);
    return result;
}


QString TAEncryption::byteToQString(const QByteArray &byte)
{
    QString result;
    if (byte.size() > 0)
    {
        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        result = codec->toUnicode(byte);
    }

    return result;
}

QByteArray TAEncryption::qstringToByte(const QString &strInfo)
{
    QByteArray result;
    if (strInfo.length() > 0)
    {
        QTextCodec *codec = QTextCodec::codecForName("utf-8");
        result = codec->fromUnicode(strInfo);
    }
    return result;
}
#endif // TAENCRYPTION_H
