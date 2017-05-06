#ifndef TALKWINDOW_H
#define TALKWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QDateTime>
#include "numberdefine.h"
#include "typedefine.h"

namespace Ui {
  class Talkwindow;
}

class Talkwindow : public QMainWindow
{
  Q_OBJECT

public:
  QPushButton *ButtonSend;
  explicit Talkwindow(QWidget *parent = 0);
  ~Talkwindow();
  void FuncSetName(QString InputString)
  {
    TalkTo = InputString;
  }
  void FuncAppendText(QString InputString)
  {
    TextShowScreen->append(InputString);
  }
  QString FuncGetSendTo(void)
  {
    return TalkTo;
  }
  QString FuncGetTextFromScreen(void)
  {
    return TextInputScreen->toPlainText();
  }
  void FuncSetUserName(QString InputString)
  {
      UserName = InputString;
  }

  void FuncShowToScrren(QString InputString)
  {
        char TempStr[BUFFER_LENGTH+50];
//        char TempChar[NAME_LENGTH+2];
//        QString TempQStr;
//        memset(TempStr,0,BUFFER_LENGTH);
//        strcpy(TempStr,InputString.toLatin1().data());
//        memset(TempChar,0,NAME_LENGTH+2);
//        strncpy(TempChar,TempStr+8,20);
//        TempQStr = QString::fromStdString(TempChar);
//        if(TempQStr.contains(TalkTo))
//          {
//              FuncAppendText(TempStr);
//          }
        memset(TempStr,0,BUFFER_LENGTH+50);
        strcpy(TempStr,InputString.toLatin1().data());
        char Head[NAME_LENGTH+2];
        char Rear[NAME_LENGTH+2];
        char TempLengthChar[6];
        memset(TempLengthChar,0,6);
        strncpy(TempLengthChar,TempStr+2,4);
        int Length = atoi(TempLengthChar);
        QDateTime Time = QDateTime::currentDateTime();
        QString TimeStr = Time.toString("MM/dd hh:mm:ss");
        char Body[BUFFER_LENGTH];
        memset(Body,0,BUFFER_LENGTH);
        strncpy(Body,TempStr+48,Length-48);
        QString QHead,QRear;
        memset(Head,0,NAME_LENGTH+2);
        memset(Rear,0,NAME_LENGTH+2);
        strncpy(Head,TempStr+8,NAME_LENGTH);
        strncpy(Rear,TempStr+NAME_LENGTH+8,NAME_LENGTH);
        QHead = QString::fromStdString(Head);
        QRear = QString::fromStdString(Rear);
        QString Space = " ";
        if(QHead.contains(UserName) && QRear.contains(TalkTo))
          {
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
               QString ShowString = TimeStr +Space+ QString::fromStdString(PureUserName) + QString::fromStdString(" :  ")+QString::fromStdString(Body);
               FuncAppendText(ShowString);
          }
        if(QHead.contains(TalkTo) && QRear.contains(UserName))
          {
               QString ShowString = TimeStr+Space + TalkTo+ QString::fromStdString(" :  ")  +QString::fromStdString(Body);
            FuncAppendText(ShowString);
          }
  }

  void FuncSendText(void);

signals:
  void SigSendText(const QString &RetMsg);
private:
  Ui::Talkwindow *ui;
  QString TalkTo;
  QString RetMsg;
  QTextEdit *TextShowScreen;
  QTextEdit *TextInputScreen;
  QString UserName;
};

#endif // TALKWINDOW_H
