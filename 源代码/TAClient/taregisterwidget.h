#ifndef TAREGISTERWIDGET_H
#define TAREGISTERWIDGET_H

#include <QWidget>
#include<model/taconstant.h>
#include<control/taregisterctl.h>
namespace Ui {
    class TARegisterWidget;
}

class TARegisterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TARegisterWidget(QWidget *parent = 0);
    ~TARegisterWidget();
public slots:
    void resetBtnRegister(bool );    // 重置注册按钮
private slots:
    void onClickBtnRegister();  // 单击“注册”按钮
    void onClickBtnCancel();   // 单击“取消”按钮
private:
    void initRegisterWidget();
    void linkSignalWithSlot();
private:
    Ui::TARegisterWidget *ui;
    UserInformation TA_info;

    TARegisterCtl *TA_RegCtl;
};

#endif // TAREGISTERWIDGET_H
