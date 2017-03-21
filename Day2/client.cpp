#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(){
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
  //  serv_addr.sin_addr.s_addr = inet_addr("118.89.106.64");  //具体的IP地址
  //  serv_addr.sin_port = htons(9999);  //端口
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(1234);
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    char _readLine[40];
    read(sock,_readLine,sizeof(_readLine)-1);
    printf("Msg:%s\n",_readLine);
    //读取服务器传回的数据
    while(1)
    {
   	 char buffer[40];
        // write(sock,buffer,sizeof(buffer)-1);
         scanf("%s",buffer);
         if(strcmp(buffer,"!quit")==0)
         { return 0;}
         write(sock,buffer,sizeof(buffer)-1);
         for(int i=1000;i>0;i--);
         read(sock,buffer,sizeof(buffer)-1);
         printf("Msg:%s\n",buffer);
    }
     close(sock);
    //关闭套接字
    return 0;
}
