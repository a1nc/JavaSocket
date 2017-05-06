#ifndef MYSOCKET_H
#define MYSOCKET_H
#include <QObject>
#include <QDebug>
#include <winsock2.h>
#include <string>
#include <string.h>
#include <iostream>
#include "typedefine.h"
#include "numberdefine.h"

class MySocket : public QObject
{
  Q_OBJECT
private:
  SOCKET Sock;
  WSADATA WsaData;
  sockaddr_in SockAddr;
  char Buffer[BUFFER_LENGTH];
  //char TempStr[BUFFER_LENGTH];
  QString RetMsg;
  QString UserName;
signals:
  void SigGetMsg(const QString &RetMsg);
  void SigSetConnSuccess(const QString &RetMsg);
  void SigSetConnFailed(const QString &RetMsg);
  void SigCloseConnect(const QString &RetMsg);
  void SigPostSocket(const SOCKET &Sock);
  void SigPostUserName(const QString &UserName);
  void SigReceiveUserList(const QString &RetMsg);

public:
  MySocket();
  SOCKET TempSocket;
  void FuncLoginCheck(QString InputString);
  void FuncSendMsg(QString InputString);
  void FuncRequests(void)
  {
    memset(Buffer,0,BUFFER_LENGTH);
    char NoStr[]="00001699request#";
    send(Sock,NoStr,sizeof(NoStr),NULL);
  }
  void FuncRecvMsg(QString InputString)
  {
    std::cout<<"Recv: "<<InputString.toLatin1().data()<<std::endl;
    SigGetMsg(InputString);
  }

  void FuncRecvUserList(QString InputString)
  {
    memset(Buffer,0,BUFFER_LENGTH);
    recv(Sock,Buffer,BUFFER_LENGTH,NULL);
    if(Buffer[6]=='3' && Buffer[7]=='1')
      {
        std::cout<<"Loading User List Success"<<std::endl;
        std::cout<<Buffer<<std::endl;
        SigReceiveUserList(QString::fromStdString(Buffer));
      }
  }
};

#endif // MYSOCKET_H
