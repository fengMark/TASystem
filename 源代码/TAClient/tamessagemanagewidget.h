#ifndef TAMESSAGEMANAGEWIDGET_H
#define TAMESSAGEMANAGEWIDGET_H

#include <QWidget>
#include<model/taconstant.h>
#include<QVector>
#include<tamainwidget.h>
#include<talocalmessagewidget.h>
#include<tanetworkmessagewidget.h>
class TAMessageManageWidget : public QWidget
{
    Q_OBJECT
public:
    TAMessageManageWidget(TAMainWidget *, QWidget *parent = 0);
    ~TAMessageManageWidget();

    void setLocalMessageWidget(const QVector<TalkMessage> &mesVec);
    void setNetworkMessageWidget(const QVector<TalkMessage> &mesVec);

    void setListModelGroup();

signals:

public slots:

private:
    TAMainWidget * m_mainWidget;
    QTabWidget *m_tabWidget;
    TALocalMessageWidget *m_localWidget;
    TANetWorkMessageWidget *m_networkWidget;
};

#endif // TAMESSAGEMANAGEWIDGET_H
