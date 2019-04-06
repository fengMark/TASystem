#ifndef TALOGINCTL_H
#define TALOGINCTL_H

#include <QObject>
#include<model/taconstant.h>

class TATcpSocket;
class TALoginCtl : public QObject
{
    Q_OBJECT
public:
    explicit TALoginCtl(QObject *parent = 0);
    ~TALoginCtl();
    void login(const QString &id ,const QString & pwd = "",const int status = ONLINE);
    void getQuestionAndAnswer(const QString &id);
signals:
    void getLoginMessgae(const QString &, bool isLogin = false,const UserInformation * me = 0);
    void getQuestionAndAnswerSuccess(const TempStrings& tmpStr);
public slots:
    void changeLoginMessage(const QString &mes, bool isLogin);
private slots:
    void requestGetQuestionAndAnswer();
    void requestLogin();
    void sendRequest();
    void readMessage();
private:
    int m_kind;
    quint16 m_blockSize;
    TATcpSocket *m_tcpSocket;
    LoginInformation m_loginInfo;
    UserInformation m_myself;
    TempStrings m_tempStr;
    QString m_id;
};

#endif // TALOGINCTL_H
