#ifndef TACLIENTFILECTL_H
#define TACLIENTFILECTL_H

#include <QObject>
#include <QFile>
#include<QIODevice>
#include<QTextBrowser>
#include<QTextDocument>
class TAClientFileCtl : public QObject
{
    Q_OBJECT
public:
    explicit TAClientFileCtl(QObject *parent = 0);
    // 写自动登录的文件
    static bool writeAutoLogin(const QString &fileName, const QString &id,const QString &pwd, const bool isAuto = false);
    // 读自动登录的文件
    static bool readAutoLogin(const QString &fileName,QString &id,QString &pwd, bool &isAuto);

    // 删除文件
    static bool remove(const QString &filename);

    static QString simplifyRichText(QString richText);

    static void saveTalkHistory(const QString filePath, const QString fileName,
                                const QTextBrowser *messagaShow);
    static void readTalkHistory(const QString filePath, const QString fileName,
                                QTextBrowser *messagaShow);
};

#endif // TACLIENTFILECTL_H
