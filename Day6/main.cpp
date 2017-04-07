#include "loginwindow.h"
#include <QApplication>
#include <QObject>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow w;
    QObject::connect(w.LoginButton,&QPushButton::clicked,&w,&LoginWindow::GetName);
    QObject::connect(w.LoginButton,&QPushButton::clicked,&w,&LoginWindow::GetPswd);
    QObject::connect(w.LoginButton,&QPushButton::clicked,&w,&LoginWindow::UserWindowShow);
    w.show();
    return a.exec();
}

