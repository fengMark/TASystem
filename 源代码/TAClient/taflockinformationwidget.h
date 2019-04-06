#ifndef TAFLOCKINFORMATIONWIDGET_H
#define TAFLOCKINFORMATIONWIDGET_H

#include <QWidget>
#include <model/taconstant.h>
class QPushButton;
class QLabel;
class QLineEdit;
class TAMainWidget;
namespace Ui {
    class TAFlockInformationWidget;
}

class TAFlockInformationWidget : public QWidget
{
    Q_OBJECT
public:
    TAFlockInformationWidget(TAMainWidget *mainWidget, QWidget *parent = 0);
    ~TAFlockInformationWidget();
    void setReadOnly(bool isReadOnly, bool isEdit);
    void setFlockInformation(const FlockInformation &);
    FlockInformation getInformation();
signals:

protected:
    void closeEvent(QCloseEvent *);
public slots:
    void onClickCancelButton();
    void onClickOKButton();
private:
    void initTALoginWidget();
    Ui::TAFlockInformationWidget *ui;

     bool m_isReadOnly;
     bool m_isEdit;

     FlockInformation m_flockInf;
     TAMainWidget *m_mainWidget;

};

#endif // TAFLOCKINFORMATIONWIDGET_H
