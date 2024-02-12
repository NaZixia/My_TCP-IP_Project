#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUFF_SIZE 1024
void error_handling(const char *message);

int main(int argc,char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUFF_SIZE];
    int str_len=0;
    int idx=0,read_len=0;

    int32_t num; //用于接收传输字符的数量

    if(argc!=3){
        printf("Usage:%s<IP> <port>\n",argv[0]);
        exit(1);
    }

    sock=socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1){
        error_handling("error serv_sock");
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handling("connect() error!");

    fputs("scanf string len:",stdout);
    scanf("%d",&num);
    message[0] = (num >> 0) & 0xFF; // 最低有效字节
    message[1] = (num >> 8) & 0xFF;
    message[2] = (num >> 16) & 0xFF;
    message[3] = (num >> 24) & 0xFF; // 最高有效字节
    
    fgetc(stdin);
    fputs("scanf string data:",stdout);
    fgets(&message[4],BUFF_SIZE-4,stdin);

    write(sock,message,sizeof(message));
    str_len=read(sock,message,sizeof(message)-1);
    message[str_len]=0;

    printf("Message from server :%s \n",message);
    printf("funtion read call count :%d \n",num);
    close(sock);
    return 0;
}

void error_handling(const char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}