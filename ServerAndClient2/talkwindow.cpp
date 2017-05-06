#include "talkwindow.h"
#include "ui_talkwindow.h"
#include <iostream>
#include <QDateTime>

Talkwindow::Talkwindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Talkwindow)
{
  ui->setupUi(this);
  this->setFixedSize(450,450);
  //this->setWindowTitle(Talkwindow::FuncGetSendTo());
  TextShowScreen = new QTextEdit(this);
  TextShowScreen->setGeometry(QRect(10,20,420,230));
  TextInputScreen = new QTextEdit(this);
  TextInputScreen->setGeometry(QRect(10,300,420,50));

  ButtonSend = new QPushButton(this);
  ButtonSend->setText("Send");
  ButtonSend->setGeometry(QRect(330,360,100,25));
}

Talkwindow::~Talkwindow()
{
  delete ui;
}

void Talkwindow::FuncSendText(void)
{
    QString InputString = FuncGetTextFromScreen();
    char TempStr[BUFFER_LENGTH];
    memset(TempStr,0,BUFFER_LENGTH);
    strcpy(TempStr,"00");
    //calcLength()
    char StrLength[6];
    char TempStrLength[BUFFER_LENGTH];
    int StrBodyLength = InputString.length() + 48;
    if(StrBodyLength>=0 && StrBodyLength<10)
      {
          strcpy(StrLength,"000");
          strcat(StrLength,itoa(StrBodyLength,TempStrLength,10));
      }else if(StrBodyLength >= 10 && StrBodyLength<100)
      {
          strcpy(StrLength,"00");
          strcat(StrLength,itoa(StrBodyLength,TempStrLength,10));
      }else if(StrBodyLength >= 100 && StrBodyLength<1000)
      {
          strcpy(StrLength,"0");
          strcat(StrLength,itoa(StrBodyLength,TempStrLength,10));
      }else if(StrBodyLength >= 1000)
      {
         strcpy(StrLength,itoa(StrBodyLength,TempStrLength,10));
      }
    strcat(TempStr,StrLength);
    memset(TempStrLength,0,BUFFER_LENGTH);
    strcat(TempStr,itoa(TYPE_MSG,TempStrLength,10));
    strcat(TempStr,UserName.toLatin1().data());
    std::cout<<UserName.toLatin1().data()<<std::endl;
    char TempName[NAME_LENGTH+2];
    memset(TempName,0,NAME_LENGTH+2);
    strcpy(TempName,TalkTo.toLatin1().data());
    for(int i=0;i<20 - TalkTo.length();i++)
      {
        strcat(TempName,"+");
      }
    strcat(TempStr,TempName);
    std::cout<<TempName<<std::endl;
    strcat(TempStr,InputString.toLatin1().data());
    SigSendText(TempStr);
    QDateTime Time = QDateTime::currentDateTime();
    QString TimeStr = Time.toString("MM/dd hh:mm:ss");
    char PureUserName[NAME_LENGTH+2];
    memset(PureUserName,0,NAME_LENGTH+2);
    int Count = 0;
    for(int i=0;i<UserName.length();i++)
      {
        if(UserName[i] == '+')
        {
           break;
        }
        Count++;
      }
    strncpy(PureUserName,UserName.toLatin1().data(),Count);
    QString Space = " ";
    QString SendStr = TimeStr + Space+QString::fromStdString(PureUserName) +QString::fromStdString(" :  ") + InputString;
    FuncAppendText(SendStr);
}
