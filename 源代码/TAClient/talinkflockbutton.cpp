#include "talinkflockbutton.h"

TALinkFlockButton::TALinkFlockButton(TAFlockButton *btn, QWidget *parent) :
    TALinkButton(parent), m_flockBtn(btn)
{
    m_info = m_flockBtn->getInformation();
    setButtonStatus();
}

TALinkFlockButton::~TALinkFlockButton()
{
}
void TALinkFlockButton::setButtonStatus()
{
    QString str = QString(":/image/resource/image/flock.png");
    QPixmap pixmap;
    QIcon icon(str);
    pixmap = icon.pixmap(QSize(HEAD_SMALL_SIZE, HEAD_SMALL_SIZE));
    m_labelHead->setPixmap(pixmap);
    QString text = QString("<font color=green>%1</font>").
            arg(m_info.TA_flockName);
    m_labelInfo->setText(text);;
}
void TALinkFlockButton::onDoubleClickButton()
{
    m_flockBtn->openChatRoom();
    m_flockBtn->showChatRoom();
    emit deleteLinkButton(m_info.TA_flockID);
}
void TALinkFlockButton::ignoreMessage()
{
    emit deleteLinkButton(m_info.TA_flockID);
}
