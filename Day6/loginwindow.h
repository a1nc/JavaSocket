#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include "userwindow.h"

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
    void GetName(void);
    void GetPswd(void);
    void UserWindowShow(void);

signals:
    void PostName(const QString &retName);
    void PostPswd(const QString &retPswd);

private:
    Ui::LoginWindow *ui;
    UserWindow uw;
    QLineEdit *UserNameEdit;
    QLineEdit *UserPswdEdit;
    QLabel  *NameLabel;
    QLabel *PswdLabel;
    QString retName;
    QString retPswd;
};

#endif // LOGINWINDOW_H
