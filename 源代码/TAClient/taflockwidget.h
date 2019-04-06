#ifndef TAFLOCKWIDGET_H
#define TAFLOCKWIDGET_H

#include <QWidget>
#include<model/taconstant.h>
#include<QMap>
class TAMainWidget;
class TAFlockMemberButton;
class TAFlockMemberListWidget;
class QTextBrowser;
class TAClickLabel;
class QLabel;
class QToolButton;
class QPushButton;
class TAMainWidget;
class TATcpSocket;
namespace Ui {
    class TAFlockWidget;
}

class TAFlockWidget : public QWidget
{
    Q_OBJECT

public:
    TAFlockWidget(const FlockInformation & flock,TAMainWidget *,
                      QWidget *parent = 0);
    ~TAFlockWidget();
    // 添加群成员发来的信息
    void appendMessageShow(const TalkMessage &mes);
    // 设置聊天窗口显示信息
    void setLabelStatus();
    // 设置群信息
    void setChatInformation(const FlockInformation & flock);

    // 设置群成员名片
    void setFlockMemberRemark(const TempStrings &);

    // 获取群信息
    FlockInformation getFlockInformation() const
    {
        return m_flockInfor;
    }

    // 添加成员按钮
    bool addFlockMemberButton(const FlockMember &);

    // 删除成员按钮
    bool removeFlockMemberButton(const QString & memberID);

signals:
    void sendMessagesFromChat(TalkMessage &);
    //退出信号
    void roomQuitSignal();

public slots:

    // 设置群成员列表
    void setFlockMemberList(const QVector<FlockMember> &);

protected:
    //重载关闭事件
    void closeEvent(QCloseEvent *event);

private slots:

    // 清空
    void onClickBtnClear();
    // 关闭
    void onClickBtnClose();
    // 发送聊天信息
    void onClickBtnSend();

private:
    void initTAFlockChatWidget();
    void linkSignalWithSlot();

private:
    QLabel *m_labelHead;
    QLabel *m_labelInformation;      // 群名 群主题


    TAFlockMemberListWidget *m_memberListWidget;
    FlockInformation m_flockInfor;
    TalkMessage m_mes;

    QMap<QString, TAFlockMemberButton *> m_flockMemberMap; // 群成员ID 与 群成员按钮 映射表

    TAMainWidget *m_mainWidget;

    QVector<TalkMessage> m_messagesVec;
    Ui::TAFlockWidget *ui;
};

#endif // TAFLOCKWIDGET_H
