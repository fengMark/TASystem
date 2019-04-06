#ifndef TALINKFRIENDBUTTON_H
#define TALINKFRIENDBUTTON_H
#include<talinkbutton.h>

class TAFriendButton;
class TALinkFriendButton : public TALinkButton
{
    Q_OBJECT
public:
    TALinkFriendButton(TAFriendButton * btn,QWidget *parent = 0);
    ~TALinkFriendButton();
    void setButtonStatus();
public slots:
    void onDoubleClickButton();
    void ignoreMessage();
private:
    TAFriendButton * m_friendBtn;
    FriendInformation m_info;
};

#endif // TALINKFRIENDBUTTON_H
