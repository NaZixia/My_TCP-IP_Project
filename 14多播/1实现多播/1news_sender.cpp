#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUFF_SIZE 100

void error_handling(const char *buf);

int main(int argc,char *argv[])
{

    int send_sock;      //服务端套接字
    struct sockaddr_in mul_adr;

    int time_live=TTL;
    FILE *file;
    char buf[BUFF_SIZE];
    int str_len;

    if(argc!=3){
        printf("Usage:%s<group IP> <port>\n",argv[0]);
        exit(1);
    }

    send_sock=socket(PF_INET,SOCK_DGRAM,0);
    if(send_sock==-1){
        error_handling("error UDP send_sock cretion");
    }

    memset(&mul_adr,0,sizeof(mul_adr));
    mul_adr.sin_family=AF_INET;
    mul_adr.sin_addr.s_addr=inet_addr(argv[1]);
    mul_adr.sin_port=htons(atoi(argv[2]));

    setsockopt(send_sock,IPPROTO_IP,IP_MULTICAST_TTL,
    (void *)&time_live,sizeof(time_live));
    if((file=fopen("news.txt","r"))==NULL)
        error_handling("fopen error");

    while(!feof(file)){
        fgets(buf,BUFF_SIZE,file);
        sendto(send_sock,buf,sizeof(buf),0,
            (struct sockaddr*)&mul_adr,sizeof(mul_adr));
        sleep(2);
    }
    fclose(file);
    close(send_sock);
    return 0;

}

void error_handling(const char *buf){
    fputs(buf,stderr);
    fputc('\n',stderr);
    exit(1);
}
