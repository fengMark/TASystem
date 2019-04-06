#ifndef TALINKFLOCKBUTTON_H
#define TALINKFLOCKBUTTON_H
#include<talinkbutton.h>
#include<taflockbutton.h>
class TALinkFlockButton : public TALinkButton
{
    Q_OBJECT
public:
    TALinkFlockButton(TAFlockButton * btn, QWidget *parent = 0);
    ~TALinkFlockButton();

    // 设置button显示信息
    void setButtonStatus();

signals:

public slots:
    // 左键 双击
    void onDoubleClickButton();
    // 忽略消息
    void ignoreMessage();

private:
    TAFlockButton *m_flockBtn;
    FlockInformation m_info;
};

#endif // TALINKFLOCKBUTTON_H
