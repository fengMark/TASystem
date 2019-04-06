#ifndef TALOGINWIDGET_H
#define TALOGINWIDGET_H

#include <QWidget>

#include<model/taconstant.h>

class  TALoginCtl;
class TAMainCtl;
class TAClientFileCtl;
class TAMainWidget;
class QLineEdit;
class QHBoxLayout;
class TAClickLabel;
class QLabel;
namespace Ui {
    class TALoginWidget;
}

class TALoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TALoginWidget(QWidget *parent = 0);
    ~TALoginWidget();
public slots:
    void setLabelStatus(const QString &mes, bool isLogin,const UserInformation *me = 0);
protected:
    // 处理定时器事件
    void timerEvent(QTimerEvent * event);
private:
    void initLoginwidget();
    void linkSignalAndSlot();
    void closeTimer();
    void setServer(const QString &ip = "127.0.0.1", const int port = 1234);
    bool getisMemUIDwithPWD();//记住密码
    void readUserIDandPassword();
private slots:
     void onClickButtonLogin();
     void onClickLabelRegister();
     void onClickLabelForgot();
private:
    Ui::TALoginWidget *ui;
    TAClickLabel *TARegisterLabel;
    TAClickLabel *TAForgetPwdLabel;
    QLineEdit *TA_hostAddress;
    QLineEdit *TA_hostPort;
    QWidget *TA_extendedWidget;
    QHBoxLayout *hLayoutExtended;
    QLabel *TA_labelStatus;

    TALoginCtl *TA_loginCtl; // 用于控制登录

    int     TA_timerID;            // 定时器ID
    bool    TA_closeTimer;        // 定时器是否关闭
    int     TA_counter;            // 计数器
    int     TA_status;
    bool    isLogin;            // 是否能登录
};

#endif // TALOGINWIDGET_H
