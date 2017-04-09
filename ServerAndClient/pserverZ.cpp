#include<stdlib.h>
#include<pthread.h>
#include<sys/socket.h>
#include<sys/types.h>       
#include<stdio.h>
#include<netinet/in.h>      //structure sockaddr_in
#include<arpa/inet.h>       //Func : htonl; htons; ntohl; ntohs
#include<assert.h>          //Func :assert
#include<string.h>          //Func :memset
#include<unistd.h>          //Func :close,write,read

#define SOCK_SERVER_IP 127.0.0.1
#define SOCK_PORT 9988
#define BUFFER_LENGTH 1024
#define MAX_CONN_LIMIT 512   //max connection limit
#define MAX_USER_NUM 512

static void* Data_handle(void * sock_fd);

struct UserInfo
{
    char Name[20];
	char Pswd[20];
	char FriList[MAX_USER_NUM][20];
	char MsgRecord[200][BUFFER_LENGTH];
	int FriendNum;
	int RecordNum;
};
UserInfo UserList[MAX_USER_NUM];
int LoadCount = 0;

//Func
int GetUserID(char *);
int LoadRecord(void);
int LoginCheck(char* ,void * );//return UserId
void SendLeftMsg(int ,void *);

int main()
{
    int ServerSocket;
    int ClientScoket;
    struct sockaddr_in SocketAddrIn;
    struct sockaddr_in SocketAddrClient;
    ServerSocket = socket(AF_INET,SOCK_STREAM,0);//ipv4
    memset(&SocketAddrIn,0,sizeof(SocketAddrIn));
    SocketAddrIn.sin_family = AF_INET;
    SocketAddrIn.sin_addr.s_addr = inet_addr("127.0.0.1");
    SocketAddrIn.sin_port = htons(SOCK_PORT);
    bind(ServerSocket,(struct sockaddr*)(&SocketAddrIn),sizeof(SocketAddrIn));
    listen(ServerSocket,MAX_CONN_LIMIT);

    //Loading Data  File...
    LoadRecord();

    while(1)
    {//waiting for new connection
        printf("SYSTEM: Waiting for new connection...\n");
        pthread_t thread_id;
        int ClientLength = sizeof(SocketAddrClient);
        ClientScoket = accept(ServerSocket,(struct sockaddr*)(&SocketAddrClient),(socklen_t*)(&ClientLength));
        if(ClientScoket == -1)
        {
            printf("SYSTEM: Accept Error\n");
            continue;
        }
        printf("SYSTEM: A new Connection Occurs\n");
        if(pthread_create((&thread_id),NULL,&Data_handle,&ClientScoket) == -1)
        {
            printf("SYSTEM: Pthread Create Error");
            break;
        }
    }
    int ret = shutdown(ServerSocket,SHUT_WR);
    assert(ret != 1);
    return 0;
}

static void* Data_handle(void *SockIn)
{
    int PthSock = *((int *)SockIn);
    int FlagRecv = 0;
    int ID=-1;
    char DataRecv[BUFFER_LENGTH];
    char DataSend[BUFFER_LENGTH];

    memset(DataRecv,0,BUFFER_LENGTH);
    int LoginNum = read(PthSock,DataRecv,BUFFER_LENGTH);
    ID = LoginCheck(DataRecv,&PthSock);

    //wating for message
    printf("SYSTEM: Now Wating for Msg\n");
    while(1)
    {
        int i=0;
        //printf("zzzzzz\n");
        
        SendLeftMsg(ID,&PthSock);
        memset(DataRecv,0,BUFFER_LENGTH);
        if(read(PthSock,DataRecv,BUFFER_LENGTH)>0)
        {//if get the Msg
            if(strcmp(DataRecv,"test")!=0)
            {
                //memset(DataSend,0,BUFFER_LENGTH);
                char NameTo[20];
                char MsgTo[BUFFER_LENGTH];
                int NameCount=0;
                int MsgCount=0;
                int SendID=0;
                int NameFlag=1;
                memset(MsgTo,0,BUFFER_LENGTH);
                for(int i=0;i<strlen(DataRecv);i++)
                {
                    if(DataRecv[i]=='@')
                    {
                        NameFlag=0;
                        i++;
                    }
                    if(NameFlag==1)
                    {
                        NameTo[NameCount]=DataRecv[i];
                        NameCount++;
                    }
                    if(NameFlag==0)
                    {
                        MsgTo[MsgCount]=DataRecv[i];
                        MsgCount++;
                    }
                }
                SendID = GetUserID(NameTo);
                if(SendID!=ID)
                {
                    printf("SYSTEM: Send To %s\n",NameTo);
                    char SendMsgFormat[BUFFER_LENGTH]={0};
                    strcpy(SendMsgFormat,UserList[ID].Name);
                    strcat(SendMsgFormat,":");
                    strcat(SendMsgFormat,MsgTo);
                    printf("SYSTEM: %s\n",SendMsgFormat);
                    int _MsgCount=UserList[SendID].RecordNum;
                    strcpy(UserList[SendID].MsgRecord[_MsgCount],SendMsgFormat);
                    UserList[SendID].RecordNum++;
                }
            }
            else
            {
                char TempStr[]="test";
                write(PthSock,TempStr,strlen(TempStr));
            }

         }

        // }else{
        //     printf("111\n");

        // }
        }
    }
//}

void SendLeftMsg(int _ID,void *_SocketIn)
{
                // struct UserInfo
                // {
                //  char Name[20];
                // 	char Pswd[20];
                // 	char FriList[MAX_USER_NUM][20];
                // 	char MsgRecord[200][BUFFER_LENGTH];
                // 	int FriendNum;
                // 	int RecordNum;
                // };
    int ID = _ID;
    //int *SocketIn = _SocketIn;
    int SocketIn = *((int *)_SocketIn);
    char DataSend[1024];
    int UserRecordCount=UserList[ID].RecordNum;
    int _count=0;
    // for(int i=0;i<10;i++)
    // {
    //     printf("dd\t");
    //     strcpy(DataSend,"AAAAAAAAAAA");
    //     write(SocketIn,DataSend,strlen(DataSend));
    //     memset(DataSend,0,strlen(DataSend));
    // }
   // printf("SYSTEM: Left %d Msg Need To Send\n",UserRecordCount);
    if(UserList[ID].RecordNum>0)
    {
        for(int i=0;i<UserRecordCount;i++)
       {
            memset(DataSend,0,BUFFER_LENGTH);
            strcpy(DataSend,UserList[ID].MsgRecord[i]);
            printf("\t\t%s\n",DataSend);
            write(SocketIn,DataSend,strlen(DataSend));
            UserList[ID].RecordNum--;
            //_count++;
            //UserList[ID].RecordNum = UserList[ID].RecordNum - _count;
        }
        
    }
}

int LoadRecord(void)
{
    FILE *fp;
    char FileName[]="UserData.log";
    if((fp = fopen(FileName,"r")) == NULL)
    {
        printf("UserDataLog Open Fail\n");
        exit(1);
    }
    printf("SYSTEM: Loading User Data...\n");
    while(fscanf(fp,"%s\t\t%s",UserList[LoadCount].Name,
                    UserList[LoadCount].Pswd)!=EOF)
                    {
                        LoadCount++;
                    }
    for(int i = 0;i<LoadCount;i++)
    {
        printf("%s %s\n",UserList[i].Name,UserList[i].Pswd);
    }
    printf("SYSTEM: Total User Num: %d \n",LoadCount);
}

int GetUserID(char *Name)
{
    for(int i=0;i<LoadCount;i++)
    {
        if(strcmp(UserList[i].Name,Name)==0)
        {return i;}
    }
    return -1;
}

int LoginCheck(char *_DataRecv,void *_SocketIn)
{
   // int SocketIn = _SocketIn;
   int SocketIn = *((int *)_SocketIn);
    char DataBuffer[BUFFER_LENGTH];
    char TempName[20];
    char TempPswd[20];
    int NameFlag=1;
    int PswdCount=0;
    int NameCount=0;
    strcpy(DataBuffer,_DataRecv);
    for (int i = 0; i < strlen(DataBuffer); i++)
		{
			if (DataBuffer[i] == '#')
			{
				NameFlag = 0;
				i++;
			}
			if (NameFlag == 1)
			{
				TempName[NameCount] = DataBuffer[i];
				NameCount++;
			}
			if (NameFlag ==0)
			{
				TempPswd[PswdCount] = DataBuffer[i];
				PswdCount++;
			}
		}
    printf("SYSTEM: Loging UserInfo Name:%s %t Pwsd:%s\n",TempName,TempPswd);
    int UserId = GetUserID(TempName);
    printf("AAAAA:%d\n",UserId);
    if(UserId == -1)
    {
        char TempStr[]="0001Password or UserName Input Error!\n";
        write(SocketIn,TempStr,strlen(TempStr));
        close(SocketIn);
        pthread_exit(NULL);
    }
    if(strcmp(UserList[UserId].Pswd,TempPswd)==0)
    {
        printf("SYSTEM: One client connect!\n");
        return UserId;
    }else
    {
        printf("zzzzzzzz\n");
        char TempStr[]="0001Password or UserName Input Error!\n";
        write(SocketIn,TempStr,strlen(TempStr));
        close(SocketIn);
        pthread_exit(NULL);
    }
    return -1;
}