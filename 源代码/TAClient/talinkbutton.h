#ifndef TALINKBUTTON_H
#define TALINKBUTTON_H

#include <QPushButton>
#include<QMenu>
#include<model/taconstant.h>
#include<QLabel>
#include<QMouseEvent>
class TALinkButton : public QPushButton
{
    Q_OBJECT
public:
    TALinkButton(QWidget *parent = 0);
    virtual ~TALinkButton(){}
    void setLatestMessage(const TalkMessage & mes);
    virtual void setButtonStatus() = 0;
signals:
    void deleteLinkButton(const QString & id);
public slots:
    virtual void onDoubleClickButton() = 0;
    virtual void onClickRightButton();
    virtual void ignoreMessage() = 0;
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *);
protected:
    QLabel *m_labelHead;
    QLabel *m_labelInfo;
    QLabel *m_labelMes;
    QMenu *m_menu;
private:
    void creatMenu();
};

#endif // TALINKBUTTON_H
