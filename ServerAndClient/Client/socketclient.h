#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H
#include <QObject>
#include <QDebug>
#include <winsock2.h>
#include "stdio.h"
#include "stdlib.h"
#include "string"
#include "string.h"
#pragma comment(lib,"ws2_32.lib")


class MySocket : public QObject
{
    Q_OBJECT
public:
    MySocket() {}
    SOCKET tsock;
    void SetConnect(QString A)
    {
         WSAStartup(MAKEWORD(2,2),&wsaData);
         sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
         memset(&sockAddr,0,sizeof(sockAddr));
         memset(szBuffer,0,sizeof(szBuffer));
         sockAddr.sin_family=PF_INET;
         sockAddr.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
         sockAddr.sin_port=htons(9988);
         ::connect(sock,(SOCKADDR*)&sockAddr,sizeof(SOCKADDR));
         tsock = sock;
         //QString A="this#asdasd";
         QByteArray Temp = A.toLatin1();
         //qDebug()<<"zxxcxccc"<<Temp;
         strcpy(tempStr,Temp.data());
         send(sock,tempStr,sizeof(tempStr),NULL);
        // recv(sock,szBuffer,MAXBYTE,NULL);
         retMsg = "连接服务器成功!";
         MsgGet(retMsg);
         //SigSetConnect(retMsg);
         ConSet();
         SocketP(sock);
    }

    void CloseConnect()
    {
         closesocket(sock);
         memset(szBuffer,0,sizeof(szBuffer));
         strcpy(szBuffer,"Socket closed");
         retMsg=QString::fromStdString(szBuffer);
         MsgGet(retMsg);
        // SigCloseConnect(retMsg);
    }

    void RecvMsg(QString S)
    {
       // int flag = recv(sock,szBuffer,MAXBYTE,NULL);
        //retMsg=QString::fromStdString(szBuffer);
        retMsg = S;
        MsgGet(retMsg);
        SigReciveMsg(retMsg);
    }

    void Requests()
    {
        char NoStr[]="test";
        send(sock,NoStr,sizeof(NoStr),NULL);
    }

    void GetSendBoxAndSend(QString A)
    {
        //qDebug()<<"++++++++++";
        QByteArray Temp = A.toLatin1();
        strcpy(tempStr,Temp.data());
        //qDebug()<<"__________"<<tempStr<<sizeof(tempStr);
        send(sock,tempStr,sizeof(tempStr),NULL);
    }

    QString SocketRetMsg(void)
    {
         return retMsg;
    }

    void SetFlag(void)
    {
        Tflag=1;
        qDebug()<<"ssssssssssssssssffffffffffff"<<Tflag;
    }
    int Tflag=0;
signals:
    void MsgGet(const QString &retMsg);
    void SigSetConnect(const QString &retMsg);
    void SigCloseConnect(const QString &retMsg);
    void SigReciveMsg(const QString &retMsg);
    void SigSokcetRetMsg(const QString &retMsg);


    void ConSet(void);
    void SocketP(const SOCKET &sock);

private:
    SOCKET sock;
    WSADATA wsaData;
    sockaddr_in sockAddr;
    char szBuffer[MAXBYTE]={0};
    QString retMsg;
    char tempStr[200];
    int a;
};

#endif // SOCKETCLIENT_H
