#include "loginwindow.h"
#include "mysocket.h"
#include <QApplication>
#include <iostream>
#include <string.h>
#include <string>

MySocket mysocket;
pthread_t pid;

static void *MsgHandle(void *);
void MsgFlush(QString InputString);

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Loginwindow w;
  w.show();
  QObject::connect(w.ButtonLogin,&QPushButton::clicked,&w,&Loginwindow::FuncFormatLogin);
  QObject::connect(&w,&Loginwindow::SigPostLoginStr,&mysocket,&MySocket::FuncLoginCheck);
  QObject::connect(&mysocket,&MySocket::SigSetConnFailed,&w,&Loginwindow::FuncShowError);
  //QObject::connect(&mysocket,&MySocket::SigSetConnSuccess,&w,&Loginwindow::FuncShowUserwindow);
  QObject::connect(&mysocket,&MySocket::SigSetConnSuccess,&mysocket,&MySocket::FuncRecvUserList);
  QObject::connect(&mysocket,&MySocket::SigReceiveUserList,&w,&Loginwindow::FuncShowUserwindow);
  QObject::connect(&mysocket,&MySocket::SigReceiveUserList,MsgFlush);
  QObject::connect(&mysocket,&MySocket::SigPostUserName,&w.uw,&Userwindow::FuncGetUserName);
  QObject::connect(&mysocket,&MySocket::SigGetMsg,&w.uw,&Userwindow::FuncReceiveMsg);
  QObject::connect(&w.uw,&Userwindow::SigTalkwindowSendMsg,&mysocket,&MySocket::FuncSendMsg);
  return a.exec();
}

void MsgFlush(QString InputString)
{
  int s;
  pthread_create(&pid,NULL,&MsgHandle,&s);
}

static void* MsgHandle(void *tt)
{
    QString retMsg;
    SOCKET PthScoket = mysocket.TempSocket;
    char szBuffer[BUFFER_LENGTH];
    int flag=0;
    char GetLength[5];
    int FrameLength = 0;
    int PacketLength = 0;
    int AheadLength = 0;
    int BreakFlag = 0;
    while(1)
    {
        flag=0;
        memset(szBuffer,0,sizeof(szBuffer));
        mysocket.FuncRequests();
        if(recv(PthScoket,szBuffer,MAXBYTE,NULL) > 0)
        {
            memset(GetLength,0,5);
            strncpy(GetLength,szBuffer+FrameLength+2,4);
            FrameLength = atoi(GetLength);
            PacketLength = strlen(szBuffer);
            //std::cout<<szBuffer<<std::endl;
            //std::cout<<"FrameLength"<<FrameLength<<"  PacketLength"<<PacketLength<<std::endl;
            while(FrameLength + AheadLength <= PacketLength)
                {
                  char *typestr;
                  typestr = new char[3];
                  memset(typestr,0,3);
                  strncpy(typestr,szBuffer+6+AheadLength,2);
                  if(typestr[0]!='9' && typestr[1]!='9')
                  {
                    //std::cout<<Count<<" "<<tempstr<<std::endl;
                    //Count++;
                    char SendMsg[1024];
                    memset(SendMsg,0,1024);
                    strncpy(SendMsg,szBuffer+AheadLength,FrameLength);
                    //retMsg=QString::fromStdString(szBuffer);
                    retMsg = QString::fromStdString(SendMsg);
                    mysocket.FuncRecvMsg(retMsg);
                  }
                  AheadLength = AheadLength + FrameLength;
                  if(AheadLength >= PacketLength)
                   {
                      FrameLength = 0;
                      AheadLength = 0;
                      BreakFlag = 1;
                      break;
                   }
                  memset(GetLength,0,5);
                  strncpy(GetLength,szBuffer+AheadLength+2,4);
                  FrameLength = atoi(GetLength);
                }
        }
        retMsg.clear();
        Sleep(100);
    }
}
