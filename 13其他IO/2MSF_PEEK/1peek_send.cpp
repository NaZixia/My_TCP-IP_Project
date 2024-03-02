#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define BUF_SIZE 100
void error_handling(const char *message);
void suc_handling(const char *message);

int main(int argc,char *argv[])
{
    int sock;
    struct sockaddr_in serv_adr;
    char message[BUF_SIZE];


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


    write(sock,"meees",sizeof("meees"));

    close(sock);
    return 0;
}

void error_handling(const char *message){
    fputs(message,stderr);
    exit(1);
}

void suc_handling(const char *message){
    fputs(message,stderr);
}
