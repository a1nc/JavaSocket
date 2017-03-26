#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>       //pthread_t , pthread_attr_t and so on.
#include<stdio.h>
#include<netinet/in.h>      //structure sockaddr_in
#include<arpa/inet.h>       //Func : htonl; htons; ntohl; ntohs
#include<assert.h>          //Func :assert
#include<string.h>          //Func :memset
#include<unistd.h>          //Func :close,write,read
#define SOCK_PORT 9988
#define BUFFER_LENGTH 1024
int main(int argc,char** args)
{
    int sockfd;
    int tempfd;
    struct sockaddr_in s_addr_in;
    char data_send[BUFFER_LENGTH];
    char data_recv[BUFFER_LENGTH];
    memset(data_send,0,BUFFER_LENGTH);
    memset(data_recv,0,BUFFER_LENGTH);

    sockfd = socket(AF_INET,SOCK_STREAM,0);//iipv4,TCP
		if(argc<2)
		{
			printf("Input [ID] [Password]\n");
			return 0;
		}
		char* ID = args[1];
		char* PD = args[2];
	  //printf("%s %s \n",ID,PD);

    if(sockfd == -1)
    {
        fprintf(stderr,"socket error!\n");
        exit(1);
    }

    //before func connect, set the attr of structure sockaddr.
    memset(&s_addr_in,0,sizeof(s_addr_in));
    s_addr_in.sin_addr.s_addr = inet_addr("127.0.0.1");      //trans char * to in_addr_t
    s_addr_in.sin_family = AF_INET;
    s_addr_in.sin_port = htons(SOCK_PORT);

    tempfd = connect(sockfd,(struct sockaddr *)(&s_addr_in),sizeof(s_addr_in));
    if(tempfd == -1)
    {
        fprintf(stderr,"Connect error! \n");
        exit(1);
    }
    //send ID and Password to Sever
    strcat(data_send,ID);
		strcat(data_send,"#");
		strcat(data_send,PD);
    //puts(data_send);
    write(sockfd,data_send,BUFFER_LENGTH);
    read(sockfd,data_send,BUFFER_LENGTH);
		if(strcmp(data_send,"Login Success!")!=0)
		{
			printf("%s\n",data_send);
			return 0;
		}
		else
		{
			printf("%s \n",data_send);
		}
    while( 1)
    {
        printf("Please input something you wanna say(input \"quit\" to quit):\n");
        gets(data_send);
        //scanf("%[^\n]",data_send);         //or you can also use this
        tempfd = write(sockfd,data_send,BUFFER_LENGTH);
        if(tempfd == -1)
        {
            fprintf(stderr,"write error\n");
            exit(0);
        }

        if(strcmp(data_send,"quit") == 0)  //quit,write the quit request and shutdown client
        {
            break;
        }
        else
        {
            tempfd = read(sockfd,data_recv,BUFFER_LENGTH);
            assert(tempfd != -1);
            printf("%s\n",data_recv);
            memset(data_send,0,BUFFER_LENGTH);
            memset(data_recv,0,BUFFER_LENGTH);
        }
    }

    int ret = shutdown(sockfd,SHUT_WR);       //or you can use func close()--<unistd.h> to close the fd
    assert(ret != -1);
    return 0;
}
