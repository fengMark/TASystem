#ifndef TANETWORKMESSAGEWIDGET_H
#define TANETWORKMESSAGEWIDGET_H
#include<tamessagewidget.h>
#include<tamainwidget.h>
class TANetWorkMessageWidget : public TAMessageWidget
{
    Q_OBJECT
public:
    TANetWorkMessageWidget(TAMainWidget *, QWidget *parent = 0);
    ~TANetWorkMessageWidget();

signals:

public slots:
    // 删除某日的信息记录
    void onClickDeleteDate();
    // 删除某好友信息记录
    void onClickDeleteOne();
    // 删除全部消息记录
    void onClickDeleteAll();
    // 导出某好友消息记录
    void onClickExportOne();
    // 导出所有消息记录
    void onClickExportAll();
    // 获取消息记录
    void onClickGetMessage();
//    // 单击好友列表项
//    void friendItemClicked(const QModelIndex & index);
private:
    TAMainWidget * m_mainWidget;
};

#endif // TANETWORKMESSAGEWIDGET_H
