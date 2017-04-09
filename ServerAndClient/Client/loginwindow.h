#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QString>
#include "userwindow.h"
#include "socketclient.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();
    QPushButton *LoginButton;
    MySocket ms;
    UserWindow uw;
    void GetName(void);
    void GetPswd(void);
    void UserWindowShow(void);
    void FormatLogin(void);

signals:
    void PostName(const QString &retName);
    void PostPswd(const QString &retPswd);
    void PostLogin(const QString &retMsgLogin);

private:
    Ui::LoginWindow *ui;

    QLineEdit *UserNameEdit;
    QLineEdit *UserPswdEdit;
    QLabel  *NameLabel;
    QLabel *PswdLabel;
    QString retName;
    QString retPswd;
    QString retMsgLogin;
    QString SendMsg;
    char TempStr[200];
};

#endif // LOGINWINDOW_H
