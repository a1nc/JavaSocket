#include "mysocket.h"

MySocket::MySocket()
{
    WSAStartup(MAKEWORD(2,2),&WsaData);
    Sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
    memset(&SockAddr,0,sizeof(SockAddr));
    memset(Buffer,0,BUFFER_LENGTH);
    SockAddr.sin_family = PF_INET;
    //SockAddr.sin_addr.S_un.S_addr = inet_addr("118.89.106.64");
    SockAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    SockAddr.sin_port = htons(SERVER_SOCKET_PORT);
    ::connect(Sock,(SOCKADDR*)&SockAddr,sizeof(SOCKADDR));
    TempSocket = Sock;
}

void MySocket::FuncLoginCheck(QString InputString)
{
  //Send Login String to Server
  strcpy(Buffer,(InputString.toLatin1().data()));
  char TempName[NAME_LENGTH+2];
  memset(TempName,0,NAME_LENGTH+2);
  strncpy(TempName,Buffer+8,NAME_LENGTH);
  send(Sock,Buffer,sizeof(Buffer),NULL);

  //Get The Respons from Server to check
  char TempStr[BUFFER_LENGTH];
  memset(TempStr,0,BUFFER_LENGTH);
  recv(Sock,TempStr,BUFFER_LENGTH,NULL);
  //std::cout<<"A "<<TempStr<<std::endl;
  if(TempStr[6]=='9' && TempStr[7]=='8')
  {
      UserName = QString::fromStdString(TempName);
      //std::cout<<"B "<<Buffer<<std::endl;
      RetMsg = "Conn Successed!";
      SigSetConnSuccess(RetMsg);
      SigPostUserName(UserName);
  }
  else
  {
      RetMsg = "Conn Failed! User Name or Pswd incorrect";
      SigSetConnFailed(RetMsg);
  }
}

void MySocket::FuncSendMsg(QString InputString)
{
    std::cout<<"AAAAAA:"<<InputString.toLatin1().data()<<std::endl;
    memset(Buffer,0,BUFFER_LENGTH);
    strcpy(Buffer,InputString.toLatin1().data());
    send(Sock,Buffer,sizeof(Buffer),NULL);
}
