#include "talinkfriendbutton.h"
#include<tafriendbutton.h>
#include<QDebug>
TALinkFriendButton::TALinkFriendButton(TAFriendButton *btn, QWidget *parent) :
    TALinkButton(parent), m_friendBtn(btn)
{
    m_info = m_friendBtn->getInformation();

    setButtonStatus();
}


TALinkFriendButton::~TALinkFriendButton()
{
}
void TALinkFriendButton::setButtonStatus()
{
    QString str = QString(":/image/resource/image/head/%1.bmp").arg(QString::number(m_info.TA_headPortrait));
    QString text, mark;

    if(m_info.TA_remarkName.isEmpty())
        mark = m_info.TA_nickname;
    else
        mark = m_info.TA_remarkName;

    QPixmap pixmap;
    QIcon icon(str);
    pixmap = icon.pixmap(QSize(HEAD_SMALL_SIZE, HEAD_SMALL_SIZE));
    m_labelHead->setPixmap(pixmap);

    text = QString("<font color=green>%1(%2)</font>").arg(m_info.TA_userID).arg(mark);
    m_labelInfo->setText(text);;
}
void TALinkFriendButton::onDoubleClickButton()
{
    if(m_friendBtn!=NULL){
        m_friendBtn->openChatRoom();

        m_friendBtn->showChatRoom();
        emit deleteLinkButton(m_info.TA_userID);

    }
}
void TALinkFriendButton::ignoreMessage()
{
    emit deleteLinkButton(m_info.TA_userID);
}
