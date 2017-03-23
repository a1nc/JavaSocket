#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024

int main()
{
        char *filename="a.mp4";
        FILE *fp=fopen(filename,"rb");
        if(fp==NULL)
        {
                printf("File Error\n");
                return 0;
        }

        int serv_sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

        struct sockaddr_in serv_addr;
        memset(&serv_addr,0,sizeof(serv_addr));
        serv_addr.sin_family=AF_INET;
        serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
        serv_addr.sin_port=htons(1235);
        bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
        listen(serv_sock,20);

        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_size=sizeof(clnt_addr);
        int clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);

        printf("Connect success\n");
        char buffer[BUF_SIZE]={0};
        int nCount;
				int cCount=0;
				int times=0;
        while((nCount=fread(buffer,1,BUF_SIZE,fp))>0)
        {
					 cCount++;
           write(clnt_sock,buffer,nCount);
           if(cCount%10240 == 0 )
					 { 
						 times++;
						 printf("%dMB send\n",10*times);
					 }
        }

        shutdown(clnt_sock,SHUT_WR);
        read(clnt_sock,buffer,BUF_SIZE);
  fclose(fp);
        close(clnt_sock);
        close(serv_sock);
        return 0;
}
