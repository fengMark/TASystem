#ifndef TALOCALMESSAGEWIDGET_H
#define TALOCALMESSAGEWIDGET_H

class TALocalMessageWidget : public TAMessageWidget
{
    Q_OBJECT
public:
    TALocalMessageWidget(TAMainWidget *, QWidget *parent = 0);
    ~TALocalMessageWidget();

signals:

public slots:
    // 删除某日的信息记录
    void onClickDeleteDate();
    // 删除某好友信息记录
    void onClickDeleteOne();
    // 删除全部消息记录
    void onClickDeleteAll();
    // 导人消息记录
    void onClickImport();
    // 导出当前好友消息记录
    void onClickExportOne();
    // 导出所有消息记录
    void onClickExportAll();
    // 获取消息记录
    void onClickGetMessage();
//    // 单击好友列表项
//    void friendItemClicked(const QModelIndex &);

private:
    TAMainWidget * m_mainWidget;
};

#endif // TALOCALMESSAGEWIDGET_H
