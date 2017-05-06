#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QStandardItemModel>
#include <QDateTime>
#include <QStandardItem>
#include "talkwindow.h"
#include "mysocket.h"
#include <iostream>

namespace Ui {
  class Userwindow;
}

class Userwindow : public QMainWindow
{
  Q_OBJECT
private:
  Ui::Userwindow *ui;
  Talkwindow talkwindow;
  QStandardItemModel *Model;
  QStandardItem *Title;
  QTreeView *TreeFriendList;
  QStandardItem *FriendList[NUMBER_FRIEND_TOTAL];
  QString UserName;
  QString RetMsg;
  char UserMsgRecord[BUFFER_LENGTH][BUFFER_LENGTH+50];
  int UserMsgCount = 0;
public:
  explicit Userwindow(QWidget *parent = 0);
  ~Userwindow();

  void FuncUserListInit(QString);
  void FuncTalkWindowShow(const QModelIndex &index);
  void FuncGetUserName(QString InputString)
  {
      UserName = InputString;
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
      this->setWindowTitle(PureUserName);
  }

  void FuncTalkwindowSendMsg(QString InputString)
  {
      QDateTime Time = QDateTime::currentDateTime();
      QString TimeStr = Time.toString("MM/dd hh:mm:ss");
      strcpy(UserMsgRecord[UserMsgCount],InputString.toLatin1().data());
      strcat(UserMsgRecord[UserMsgCount],TimeStr.toLatin1().data());
      UserMsgCount++;
      SigTalkwindowSendMsg(InputString);
  }

  void FuncReceiveMsg(QString InputString)
  {
      QDateTime Time = QDateTime::currentDateTime();
      QString TimeStr = Time.toString("MM/dd hh:mm:ss");
      strcpy(UserMsgRecord[UserMsgCount],InputString.toLatin1().data());
      strcat(UserMsgRecord[UserMsgCount],TimeStr.toLatin1().data());
      UserMsgCount++;
      std::cout<<"UserWindow: "<<InputString.toLatin1().data()<<std::endl;
      SigReceiveMsgToPost(InputString);
  }

  void FuncOpenTalkWindow(void)
  {
      for(int i=0;i<UserMsgCount;i++)
        {
          SigReceiveMsgToPost(UserMsgRecord[i]);
        }
  }

signals:
  void SigTalkwindowSendMsg(const QString &RetMsg);
  void SigReceiveMsgToPost(const QString &RetMsg);
};

#endif // USERWINDOW_H
