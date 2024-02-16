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
    char file_name[BUFF_SIZE];
    int str_len=0;
    int idx=0,read_len=0;

    FILE *fp;
    int32_t num; //用于接收传输字符的数量

    if(argc!=2){
        printf("Usage:%s <port>\n",argv[0]);
        exit(1);
    }

    sock=socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1){
        error_handling("error serv_sock");
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handling("connect() error!");

    fputs("file name:",stdout);
    scanf("%s",file_name);

    write(sock,file_name,BUFF_SIZE);

    fp= fopen("client.txt", "w"); 
    if (fp == NULL)  
    {  
        printf("File:\t%s Can Not Open To Write!\n", file_name);  
        exit(1);  
    }

    memset(message,0,BUFF_SIZE);
    str_len=read(sock,message,BUFF_SIZE-1);
    int write_length = fwrite(message, sizeof(char), str_len, fp);  

    message[str_len]=0;

    printf("Message from server :%s \n",message);
    printf("funtion read call count :%d \n",num);
    fclose(fp);  
    close(sock);
    return 0;
}

void error_handling(const char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}