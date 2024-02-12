#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(const char *message);
void suc_handling(const char *message);

int main(int argc,char *argv[])
{
    int sock;
    struct sockaddr_in serv_adr;
    char message[BUF_SIZE];
    int str_len;

    int recv_len,recv_cnt; 

    if(argc!=2){
        printf("Usage:%s<IP> <port>\n",argv[0]);
        exit(1);
    }

    sock=socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1){
        error_handling("error serv_sock");
    }

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_adr.sin_port=htons(atoi(argv[1]));

    if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1)
        error_handling("connect() error!");
    else
        puts("connect....");

    while(1){
        fputs("print q to quit\n",stdout);
        fgets(message,sizeof(message),stdin);

        if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
            break;

        
        str_len=write(sock,message,strlen(message));
        
        recv_len=0;
        while(recv_len<str_len){
            recv_cnt=read(sock,&message[recv_len],BUF_SIZE-1);
            recv_len+=recv_cnt;
        }
        message[str_len]=0;
        printf("Message from server :%s \n",message);
    }
    close(sock);
    return 0;
}

void error_handling(const char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

void suc_handling(const char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
}