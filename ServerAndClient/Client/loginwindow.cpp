#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QDebug>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    //qDebug()<<this->width()<<" "<<this->height();
    LoginButton = new QPushButton(this);
    LoginButton->setText("Lo...this");

    UserNameEdit = new QLineEdit(this);
    UserPswdEdit = new QLineEdit(this);
    UserPswdEdit->setEchoMode(QLineEdit::Password);

    NameLabel = new QLabel(this);
    NameLabel->setText("Name");
    PswdLabel = new QLabel(this);
    PswdLabel->setText("Pswd");

    NameLabel->setGeometry(QRect(70,100,50,25));
    PswdLabel->setGeometry(QRect(70,130,50,25));
    UserNameEdit->setGeometry(QRect(110,100,200,25));
    UserPswdEdit->setGeometry(QRect(110,130,200,25));
    LoginButton->setGeometry(QRect(180,200,150,25));
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::GetName(void)
{
    QString Name;
    Name = UserNameEdit->text();
    LoginWindow::retName = Name;
    //qDebug()<<Name;
    //LoginWindow::PostName(Name);
}

void LoginWindow::GetPswd(void)
{
    QString Pswd;
    Pswd = UserPswdEdit->text();
    LoginWindow::retPswd = Pswd;
    //qDebug()<<Pswd;
    //LoginWindow::PostPswd(Pswd);
}

void LoginWindow::FormatLogin(void)
{
    LoginWindow::SendMsg = retName + "#" + retPswd;
    qDebug()<<"Login Test: "<<SendMsg;
//        QByteArray Temp = SendMsg.toLatin1();
//        qDebug()<<"Login Test: "<<Temp;
//        strcpy(tempStr,Temp.data());
//        return SendMsg;
    LoginWindow::PostLogin(SendMsg);
}

void LoginWindow::UserWindowShow()
{
    LoginWindow::close();
    uw.show();
}


