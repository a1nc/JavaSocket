#include "loginwindow.h"
#include "ui_loginwindow.h"

Loginwindow::Loginwindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Loginwindow)
{
  ui->setupUi(this);
  this->setFixedSize(this->width(),this->height());
  this->setWindowTitle("C++ QtChat");

  ButtonLogin = new QPushButton(this);
  ButtonLogin->setText("Login");

  LineUserName = new QLineEdit(this);
  LineUserPswd = new QLineEdit(this);
  LineUserPswd->setEchoMode(QLineEdit::Password);

  LabelUserName = new QLabel(this);
  LabelUserName->setText("Name");
  LabelUserPswd = new QLabel(this);
  LabelUserPswd->setText("Pswd");
  LabelLoginError = new QLabel(this);

  LabelUserName->setGeometry(QRect(70,100,50,25));
  LabelUserPswd->setGeometry(QRect(70,130,50,25));
  LineUserName->setGeometry(QRect(110,100,200,25));
  LineUserPswd->setGeometry(QRect(110,130,200,25));
  ButtonLogin->setGeometry(QRect(170,200,80,25));
  LabelLoginError->setGeometry(QRect(100,250,250,25));
}

Loginwindow::~Loginwindow()
{
  delete ui;
}

void Loginwindow::FuncGetName(void)
{
  QString TempName;
  TempName = LineUserName->text();
  Name = TempName;
}

void Loginwindow::FuncGetPswd(void)
{
  QString TempPswd;
  TempPswd = LineUserPswd->text();
  Pswd = TempPswd;
}

void Loginwindow::FuncFormatLogin(void)
{
  Loginwindow::FuncGetName();
  Loginwindow::FuncGetPswd();

  char TempName[NAME_LENGTH+2];
  char TempPswd[PSWD_LENGTH+2];
  char TempStr[BUFFER_LENGTH];
  memset(TempStr,0,BUFFER_LENGTH);
  memset(TempName,0,NAME_LENGTH+2);
  memset(TempName,0,PSWD_LENGTH+2);

  int NameLength = strlen(Name.toLatin1().data());
  int PswdLength = strlen(Pswd.toLatin1().data());

  strcpy(TempName,(Name.toLatin1()).data());
  strcpy(TempPswd,(Pswd.toLatin1()).data());
  strcat(TempStr,"0000");
  strcat(TempStr,(QString::number(TYPE_LOGIN_LENGTH).toLatin1()).data());
  strcat(TempStr,(QString::number(TYPE_LOGIN).toLatin1()).data());

  for(int i=0;i<20 - NameLength;i++)
  {
      strcat(TempName,"+");
  }
  for(int i=0;i<20 - PswdLength;i++)
  {
    strcat(TempPswd,"+");
  }
  strcat(TempStr,TempName);
  strcat(TempStr,TempPswd);
  std::cout<<TempStr<<"  "<<strlen(TempStr)<<std::endl;
  LoginStr=QString::fromStdString(TempStr);
  Loginwindow::SigPostLoginStr(LoginStr);
}

void Loginwindow::FuncShowUserwindow(QString InputString)
{
  Loginwindow::close();
  uw.FuncUserListInit(InputString);
  uw.show();
}

void Loginwindow::FuncShowError(QString InputString)
{
   std::cout<<"Error "<<std::endl;
   LabelLoginError->setText("User Name or Pswd Incorrect");
}
