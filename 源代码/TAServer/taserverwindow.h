#ifndef TASERVERWINDOW_H
#define TASERVERWINDOW_H

#include <QMainWindow>
#include<control/taservermessagectl.h>
namespace Ui {
    class TAServerWindow;
}

class TAServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TAServerWindow(QWidget *parent = 0);
    ~TAServerWindow();
public slots:
    void onClickStartButton();
    void onClickStopButton();
private:
    Ui::TAServerWindow *ui;
    TAServerMessageCtl *TA_serverCtrl;
};

#endif // TASERVERWINDOW_H
