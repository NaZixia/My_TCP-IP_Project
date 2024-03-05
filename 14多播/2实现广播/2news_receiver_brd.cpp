#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFF_SIZE 100
void error_handling(const char *buf);

int main(int argc,char *argv[])
{
    int recv_sock;
    char buf[BUFF_SIZE];
    int str_len;

    struct sockaddr_in adr;

    
    if(argc!=2){
        printf("Usage:%s <port>\n",argv[0]);
        exit(1);
    }

    recv_sock=socket(PF_INET,SOCK_DGRAM,0);
    if(recv_sock==-1){
        error_handling("UDP serv_recv_sock creation error");
    }

    recv_sock=socket(PF_INET,SOCK_DGRAM,0);
    memset(&adr,0,sizeof(adr));
    adr.sin_family=AF_INET;
    adr.sin_addr.s_addr=htonl(INADDR_ANY);
    adr.sin_port=htons(atoi(argv[1]));

    if(bind(recv_sock,(struct sockaddr* )&adr,
    sizeof(adr))==-1)
        error_handling("error bind()");

    while (1)
    {
        str_len=recvfrom(recv_sock,buf,BUFF_SIZE-1,
        0,NULL,0);
        if(str_len<0)
            break;
        buf[str_len]=0;
        fputs(buf,stdout);
    }
    close(recv_sock);
    return 0;
}

void error_handling(const char *buf){
    fputs(buf,stderr);
    fputc('\n',stderr);
    exit(1);
}

