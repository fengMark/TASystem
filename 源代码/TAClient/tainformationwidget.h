#ifndef TAINFORMATIONWIDGET_H
#define TAINFORMATIONWIDGET_H

#include <QWidget>
#include<model/taconstant.h>
#include<QFile>
class QTcpServer;
class QTcpSocket;
class TAMainWidget;
class TAClickLabel;
class QHBoxLayout;


namespace Ui {
    class TAInformationWidget;
}

class TAInformationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TAInformationWidget(TAMainWidget *mainWidget,QWidget *parent = 0);
    ~TAInformationWidget();
    void setReadOnly(bool isReadOnly, bool isEdit);
    void setUserInformation(const UserInformation &user);
    UserInformation getInformation();
private slots:
    void onClickCancelButton();
    void onClickOKButton();
    void onClickHeadLabel();
    void sendIconFile(int TA_headProait);
    void sendData();
protected:
    void closeEvent(QCloseEvent *);
private:
    void linkSignalWithSlot();
    Ui::TAInformationWidget *ui;
    bool m_isReadOnly;
    bool m_isEdit;
    int m_headNum;
    QString icon_name;
    TAClickLabel *headlabel;
    UserInformation m_userInf;
    TAMainWidget *m_mainWidget;

    QTcpServer *server;
    QTcpSocket *socket;

    QFile file;//文件对象
    QString fileName;//文件名
    qint64 fileSize;//文件大小
    qint64 sendSize;//已经发送文件的大小
};

#endif // TAINFORMATIONWIDGET_H
