#include "loginwindow.h"
#include <QApplication>
#include <QObject>
#include <QDebug>
#include "socketclient.h"
#include <winsock2.h>

pthread_t pid;
SOCKET sss;

static void* MsgHandle(void *tt);
void MsgFlash(void);
void SocketPass(SOCKET SSS);
void MsgPass(QString);
MySocket ms;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow w;

//    QObject::connect(w.LoginButton,&QPushButton::clicked,&w,&LoginWindow::GetName);
//    QObject::connect(w.LoginButton,&QPushButton::clicked,&w,&LoginWindow::GetPswd);
//    QObject::connect(w.LoginButton,&QPushButton::clicked,&w,&LoginWindow::FormatLogin);
//    QObject::connect(&w,&LoginWindow::PostLogin,&w.ms,&MySocket::SetConnect);


//    QObject::connect(&w.ms,&MySocket::ConSet,&MsgFlash);
//    //QObject::connect(&MsgPass,&w.ms,&MySocket::RecvMsg);
//    QObject::connect(&w.ms,&MySocket::ConSet,&w,&LoginWindow::UserWindowShow);
//    QObject::connect(&w.ms,&MySocket::SocketP,&SocketPass);
//    QObject::connect(&w.uw,&UserWindow::TWSendMsg,&w.ms,&MySocket::GetSendBoxAndSend);
//    QObject::connect(&w.ms,&MySocket::SigReciveMsg,&w.uw,&UserWindow::TWGetMsg);

    //a b c
    //ab bc
    QObject::connect(w.LoginButton,&QPushButton::clicked,&w,&LoginWindow::GetName);
    QObject::connect(w.LoginButton,&QPushButton::clicked,&w,&LoginWindow::GetPswd);
    QObject::connect(w.LoginButton,&QPushButton::clicked,&w,&LoginWindow::FormatLogin);
    QObject::connect(&w,&LoginWindow::PostLogin,&ms,&MySocket::SetConnect);


    QObject::connect(&ms,&MySocket::ConSet,&MsgFlash);
    //QObject::connect(&MsgPass,&w.ms,&MySocket::RecvMsg);
    QObject::connect(&ms,&MySocket::ConSet,&w,&LoginWindow::UserWindowShow);
  //  QObject::connect(&ms,&MySocket::SocketP,&SocketPass);
    QObject::connect(&w.uw,&UserWindow::TWSendMsg,&ms,&MySocket::GetSendBoxAndSend);
    QObject::connect(&ms,&MySocket::SigReciveMsg,&w.uw,&UserWindow::TWGetMsg);

    w.show();
    return a.exec();
}

void SocketPass(SOCKET SSS)
{
  sss = SSS;
}

void MsgPass(QString A)
{
  QString retMsg = A;
}

void MsgFlash(void)
{
   int S;
   qDebug()<<"MsgFlash";
   pthread_create(&pid,NULL,&MsgHandle,&S);
}

static void* MsgHandle(void *tt)
{
    int zz = *((int *)tt);
    QString retMsg;
    SOCKET ts = ms.tsock;
    char szBuffer[1024]={'`'};
    std::string a="";
    std::string b="";
    int i=0;
    int flag=0;
    while(1)
    {
        flag=0;
        memset(szBuffer,'`',sizeof(szBuffer));
        ms.Requests();
//        char NoStr[]="test";
//        qDebug()<<send(sss,NoStr,sizeof(NoStr),NULL);
        if(recv(ts,szBuffer,MAXBYTE,NULL) > 0)
        {
            for(i=0;i<strlen(szBuffer);i++)
            {
                if(szBuffer[i]!='`')
                {
                    a.push_back(szBuffer[i]);
                }else break;
            }
            if(a[0]=='t' && a[1]=='e' && a[2]=='s' && a[3]=='t' )
            {
                for(int j=0;j<a.length()-4;j++)
                {
                    b.push_back(a[j+4]);
                }
                flag == 1;
            }
            else
            {
                b.clear();
                qDebug()<<"a: "<<QString::fromStdString(a);
                b=a;
                qDebug()<<"b: "<<QString::fromStdString(a);
            }
                retMsg=QString::fromStdString(b);
                if(retMsg != "test" || flag==1)
                {
                    if(retMsg.length()>0)
                    {
                        ms.RecvMsg(retMsg);
                        MsgPass(retMsg);
                        qDebug()<<"szBuffer: "<<QString::fromStdString(szBuffer);
                        qDebug()<<"b:"<<QString::fromStdString(b)<<"  a:"<<QString::fromStdString(a)<<"  retMsg:"<<retMsg;
                    }
                }
        }
        b="";
        a="";
        retMsg.clear();
        Sleep(100);
    }
}
