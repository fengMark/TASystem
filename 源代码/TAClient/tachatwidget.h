#ifndef TACHATWIDGET_H
#define TACHATWIDGET_H

#include <QWidget>
#include<QTcpServer>
#include<QFileDialog>
#include<QFile>
#include<QMessageBox>
#include<model/taconstant.h>
#include<QTcpSocket>
#include<QCloseEvent>
#include<model/tasendudpsocket.h>
#include<model/tareiverudpsockeet.h>
namespace Ui {
    class TAChatWidget;
}

class TAChatWidget : public QWidget
{
    Q_OBJECT

public:
    TAChatWidget(const ChatInformation & chatInf,QWidget *parent = 0);

    ~TAChatWidget();
    // 添加好友发来的信息
    void appendMessageShow(const TalkMessage &mes);
    // 有接收文件请求需要处理
    void pendingFile(const TalkMessage &mes);
    // 对方拒绝文件发送
    void refusedSendFile();
    // 设置聊天窗口显示信息
    void setLabelStatus();
    // 设置对方信息
    void setChatInformation(const ChatInformation & chatInfo);
    // 设置对方IP
    void setFriendIP(const QString & ip);
    void setReceiveFileName(const QString &filename);
    void showReceiveFileProcess();
    void startSending(const TalkMessage &mes);
    void showRefused();
signals:
    void sendMessagesFromChat(TalkMessage &);
    void roomQuitSignal();
    void SendFinished();
    void ReceiveFinished();
protected:
    void closeEvent(QCloseEvent *event);
public slots:
    void showSendClosedConnect();
    void showReceiveClosedConnect();
    void onClickBtnFile();
    void onClickBtnClear();
    void onClickBtnClose();
    void onClickBtnSend();
    void updateSendProgress(qint64 numBytes);
    void updateReceiveProgress(qint64 numBytes);
private:
    void linkSignalWithSlot();
    void setWidgetToolBtn();
    void initsender();
    void initreceiver();
private:
    Ui::TAChatWidget *ui;
    ChatInformation TA_chatInfor;
    TalkMessage TA_mes;

    qint64 TA_Receive_totalBytes;
    qint64 TA_Send_totalBytes;

    qint64 TA_has_Receive;
    qint64 TA_has_Send;

    TAReiverUdpSockeet *receiver;
    TASendUdpSocket *sender;
    QString fileName;
    QString TA_filePath;
    QString TA_fileName;
    bool TA_isStopped;
    QFile *TA_localFile;
    qint64 TA_bytesWritten;
    qint64 TA_bytesToWrite;
    qint64 TA_loadSize;
    QByteArray TA_outBlock;   // 缓存一次发送的数据
    QTime TA_time;            // 计时器
    quint16 TA_blockSize;
    qint64 TA_bytesReceived;
    qint64 TA_bytesToReceive;
    qint64 TA_fileNameSize;
    QByteArray TA_inBlock;
};

#endif // TACHATWIDGET_H
