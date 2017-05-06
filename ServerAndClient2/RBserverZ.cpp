#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>      //structure sockaddr_in
#include<arpa/inet.h>       //Func : htonl; htons; ntohl; ntohs
#include<unistd.h>          //Func :close,write,read
#include<stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include<iostream>
#include<map>
#include<string>
#include<string.h>
#include<fstream>

using namespace std;

#define SOCK_SERVER_IP 127.0.0.1
#define SOCK_PORT 9988
#define BUFFER_LENGTH 1024
#define MAX_CONN_LIMIT 512   //max connection limit
#define MAX_USER_NUM 512

static void* PthreadFunc(void *_Param);

char *myitoa(int value,char *string,int radix)   
{   
   int rt=0;   
   if(string==NULL)   
      return NULL;   
   if(radix<=0||radix>30)   
      return NULL;   
   rt=snprintf(string,radix,"%d",value);   
   if(rt>radix)   
      return NULL;   
   string[rt]='\0';   
   return string;   
}  

struct UserInfo
{
    char Name[20];
    char Pswd[20];
    char FriList[MAX_USER_NUM][20];
    char MsgRecord[200][BUFFER_LENGTH];
    int FriendNum;
    int RecordNum;
    int Type; //Type=0 SuperUser Type=1 CommonUser
};


typedef map<std::string,UserInfo> UserList;

//UserInfo UserList[MAX_USER_NUM];

class Server
{
private:
    //int ClientSocket;
    struct sockaddr_in SSAddr;
    //struct sockaddr_in CSAddr;
public:
    Server(){;}
    int ServerSocket;
    UserList userlist;
    void SetServerSocket(void);
    int isUser(char *);
    int LoadRecord(void);
    int LoginCheck(char *,void *);
    void SendLeftMsg(char * ,void *);
    void SendFriendList(char *,void *);
    Server(const Server &obj)
    {
        SSAddr = obj.SSAddr;
        ServerSocket = obj.ServerSocket;
        userlist = obj.userlist;
    }
};



struct Param
{
    int Socket;
    Server *ClassServer;
};

void Server::SetServerSocket(void)
{
    struct sockaddr_in SSAddr;
    ServerSocket = socket(AF_INET,SOCK_STREAM,0);//ipv4
    memset(&SSAddr,0,sizeof(SSAddr));
    SSAddr.sin_family = AF_INET;
    SSAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    SSAddr.sin_port = htons(SOCK_PORT);
    bind(ServerSocket,(struct sockaddr*)(&SSAddr),sizeof(SSAddr));
    cout<<"SYSTEM: creater ServerSocket success!\n"<<endl;
}

int Server::isUser(char *_name)
{
    char TempName[20];
    strcpy(TempName,userlist[_name].Name);
    if(strlen(TempName)==0)
    {
        return -1;//not user
    }
    else
    {
        return 1;//is user
    }
}

int Server::LoadRecord(void)
{
    ifstream fp;
    char FileName[]={"UserData.log"};
    fp.open(FileName,ios::in);
    UserInfo TempUser[20];
    char TempFriName[20];
    char TempPswd[20];
    char TempName[20];
    if(fp.fail())
    {
        cout<<"SYSTEM: Open UserData.log failed during LOADRECORD"<<endl;
        return -1;
    }
    else
    {
        cout<<"SYSTEM: Loading User Data..."<<endl;
        cout<<"SYSTEM: UserList Information:"<<endl;
        int Count = 0;
        while(!fp.eof())
        {
            fp>>TempUser[Count].Name>>TempUser[Count].Pswd;
            fp>>TempUser[Count].FriendNum;
            //cout<<TempUser[Count].Name<<" "<<TempUser[Count].Pswd<<" "<<TempUser[Count].FriendNum<<endl;
            if(TempUser[Count].FriendNum>0)
            {
                for(int i=0;i<TempUser[Count].FriendNum;i++)
                {
                    memset(TempFriName,0,20);
                    fp>>TempFriName;
                    strcpy(TempUser[Count].FriList[i],TempFriName);
                }
                //cout<<"more than 1"<<endl;
            }
            memset(TempName,0,20);
            strcpy(TempName,TempUser[Count].Name);
            char TTstr[20];
            memset(TTstr,0,20);
            strcpy(TTstr,TempName);
            for(int k=0;k<19-strlen(TTstr);k++)
            {
                strcat(TempName,"+");
            }
            strcpy(TempUser[Count].Name,TempName);
            //memset(TTstr,0,20);
            strcpy(TempPswd,TempUser[Count].Pswd);
            for(int k=0;k<19-strlen(TempPswd);k++)
            {
                strcat(TempUser[Count].Pswd,"+");
            }
            userlist.insert(map<std::string,UserInfo>::value_type(TempName,TempUser[Count]));
            cout<<"\tUserName: "<<userlist[TempName].Name<<"  UserPswd: "<<userlist[TempName].Pswd<<endl;
            Count++;
        }
    }
    return 1;
}

int Server::LoginCheck(char *_DataRecv,void *_Socket)
{
    cout<<"Check1"<<endl;
    cout<<(_DataRecv)<<endl;
    int SocketIn = *((int *)_Socket);
    cout<<"Check2"<<endl;
    char DataBuffer[BUFFER_LENGTH];
    char TempName[20];
    char TempPswd[20];
    strcpy(DataBuffer,_DataRecv);
     cout<<"Check3"<<endl;
    strncpy(TempName,DataBuffer+8,19);
     cout<<"Check4"<<TempName<<endl;
    strncpy(TempPswd,DataBuffer+8+20,19);
     cout<<"Check5"<<TempPswd<<endl;
     //cout<<"FUCK "<<this->userlist[TempName].Name<<endl;
     cout<<"FUCKME"<<endl;
    if(strcmp(this->userlist[TempName].Pswd,TempPswd)==0)
    {
        cout<<"SYSTEM: LoginCheck Passed! An user connected"<<endl;
        char TempStr[]="00001698success#";  
        cout<<"SYSTEM: "<<TempStr<<endl;
        write(SocketIn,TempStr,sizeof(TempStr));
        return 1;
    }
    else
    {
        char TempStr[]="00001597failed#";
        write(SocketIn,TempStr,sizeof(TempStr));
        cout<<"SYSTEM: "<<TempStr<<endl;
        cout<<"SYSTEM: LoginCheck Failed! Pswd or UserName incorrect"<<endl;
        return -1;
    }
}

void Server::SendLeftMsg(char *_Name,void *_Socket)
{
    //cout<<"SendLeftMsgFunc"<<endl;
    int SocketIn = *((int *)_Socket);
    char DataSend[1024];
    int MsgCount = userlist[_Name].RecordNum;
    //cout<<MsgCount<<endl;
    if(userlist[_Name].RecordNum>0)
    {
        for(int i=0;i<MsgCount;i++)
        {
            memset(DataSend,0,BUFFER_LENGTH);
            strcpy(DataSend,userlist[_Name].MsgRecord[i]);
            write(SocketIn,DataSend,strlen(DataSend));
            userlist[_Name].RecordNum = userlist[_Name].RecordNum - 1; 
        }
    }
}

void Server::SendFriendList(char *_Name,void *_Socket)
{
            //     struct UserInfo
            // {
            //     char Name[20];
            //     char Pswd[20];
            //     char FriList[MAX_USER_NUM][20];
            //     char MsgRecord[200][BUFFER_LENGTH];
            //     int FriendNum;
            //     int RecordNum;
            //     int Type; //Type=0 SuperUser Type=1 CommonUser
            // };
    int SocketIn = *((int *)_Socket);
    char DataSend[1024];
    char CharList[1024];
    memset(CharList,0,1024);
    memset(DataSend,0,1024);
    int FriendNum = userlist[_Name].FriendNum;
    for(int i=0;i<FriendNum;i++)
    {
        strcat(CharList,userlist[_Name].FriList[i]);
        for(int j=0;j<20 - strlen(userlist[_Name].FriList[i]);j++)
        {
            strcat(CharList,"+");
        }
    }
    int LengthCharList = strlen(CharList)+8;
    strcpy(DataSend,"00");
    char TempChar[6];
    memset(TempChar,0,6);
    if(LengthCharList>=0 && LengthCharList<10)
    {
        strcat(DataSend,"000");
        //strcat(TempChar,myitoa(LengthCharList));
        myitoa(LengthCharList,TempChar,10);
    }else if(LengthCharList>=10 && LengthCharList<100)
    {
        strcat(DataSend,"00");
        //strcat(TempChar,myitoa(LengthCharList));
        myitoa(LengthCharList,TempChar,10);
    }else if(LengthCharList>=100 && LengthCharList<1000)
    {
        strcat(DataSend,"0");
        //strcat(TempChar,myitoa(LengthCharList));
        myitoa(LengthCharList,TempChar,10);
    }else if(LengthCharList>=1000)
    {
        //strcpy(TempChar,myitoa(LengthCharList));
        myitoa(LengthCharList,TempChar,10);
    }
    strcat(DataSend,TempChar);
    strcat(DataSend,"31");
    strcat(DataSend,CharList);
    cout<<"UserList: "<<DataSend<<endl;
    write(SocketIn,DataSend,strlen(DataSend));
}


int main()
{
    Server server;
    server.SetServerSocket();
    listen(server.ServerSocket,MAX_CONN_LIMIT);
    server.LoadRecord();
    while(1)
    {
        int ClientSocket;
        struct sockaddr_in CSAddr;
        cout<<"SYSTEM: Waiting for enw connection..."<<endl;
        pthread_t thread_id;
        int CSLen = sizeof(CSAddr);
        ClientSocket = accept(server.ServerSocket,(struct sockaddr*)(&CSAddr),(socklen_t*)(&CSLen));
        if(ClientSocket == -1)
        {
            cout<<"SYSTEM: Accept Error"<<endl;
            continue;
        }
        cout<<"SYSTEM: A new connection occurs"<<endl;
        struct Param TempP;
        TempP.Socket = ClientSocket;
        TempP.ClassServer = &server;
        cout<<"HHHHH1"<<endl;
        if(pthread_create((&thread_id),NULL,&PthreadFunc,&(TempP))==-1)
        {
            cout<<"SYSTEM: Pthread Create Error"<<endl;
            break;
        }
        shutdown(server.ServerSocket,SHUT_WR);
    }
    return 0;
}

//static void* PthreadFunc(void *_Socket)
static void* PthreadFunc(void *_Param)
{
    Param *A ;
    A=(struct Param *)_Param;
    int PthSocket = (*A).Socket;
    Server *PthServer = (*A).ClassServer;
    //Server PthServer;
    int FlagRecv = 0;
    char TempName[20];
    char DataRecv[BUFFER_LENGTH];
    char DataSend[BUFFER_LENGTH];
    memset(DataRecv,0,BUFFER_LENGTH);
    read(PthSocket,DataRecv,BUFFER_LENGTH);
    cout<<"Data"<<DataRecv<<endl;
    //int LoginFlag = PthServer->LoginCheck(DataRecv,&PthSocket);
    while(PthServer->LoginCheck(DataRecv,&PthSocket) == -1)
    {//zzzzzzzzzzzzzzzzzzzz
        memset(DataRecv,0,BUFFER_LENGTH);
        read(PthSocket,DataRecv,BUFFER_LENGTH);
    }
    strncpy(TempName,DataRecv+8,19);
    cout<<"SYSTEM: Now Wating for Msg:"<<TempName<<endl;
    cout<<"SYSTEM: Send UserList To User"<<endl;
    PthServer->SendFriendList(TempName,&PthSocket);
    while(1)
    {
        PthServer->SendLeftMsg(TempName,&PthSocket);
        memset(DataRecv,0,BUFFER_LENGTH);
        //cout<<"ReadMsg"<<endl;
        if(read(PthSocket,DataRecv,BUFFER_LENGTH)>0)
        {
            char TempStr[BUFFER_LENGTH];
            memset(TempStr,0,BUFFER_LENGTH);
            strncpy(TempStr,DataRecv+6,2);
            if(strcmp(TempStr,"99")!=0)
            {
                char SendTo[20];
                char SendFrom[20];
                char SendMsg[1024];
                char LengthFrame[4];
                int Length;
                strncpy(LengthFrame,DataRecv+2,4);
                Length = atoi(LengthFrame);
                memset(SendMsg,0,1024);
                strncpy(SendTo,DataRecv+28,19);
                strncpy(SendFrom,DataRecv+8,19);
                strncpy(SendMsg,DataRecv+48,Length-48);
                if(strcmp(SendTo,SendFrom)!=0)
                {
                    cout<<"SYSTEM: "<<SendFrom<<"send msg to "<<SendTo<<endl;
                    cout<<"\t:"<<SendMsg<<endl;
                }
                int RN = PthServer->userlist[SendTo].RecordNum;
                strcpy(PthServer->userlist[SendTo].MsgRecord[RN],DataRecv);
                PthServer->userlist[SendTo].RecordNum = PthServer->userlist[SendTo].RecordNum + 1;
            }
            else
            {
                char TempStr[BUFFER_LENGTH];
                memset(TempStr,0,BUFFER_LENGTH);
                strcpy(TempStr,"00001699request#");
                //cout<<"ignore reguest"<<endl;
                write(PthSocket,TempStr,strlen(TempStr));
            }
        }
        else
        {
            // char TempStr[BUFFER_LENGTH];
            // memset(TempStr,0,BUFFER_LENGTH);
            // strcpy(TempStr,"00001699request#");
            // cout<<"ignore reguest"<<endl;
            // write(PthSocket,TempStr,strlen(TempStr));
        }
    }
}
