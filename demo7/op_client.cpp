#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4
void error_handling(const char *message);
void suc_handling(const char *message);

int main(int argc,char *argv[])
{
    int sock;
    char opsmg[BUF_SIZE];
    int result,opnd_cnt,i;
    struct sockaddr_in serv_adr;


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

    fputs("print q to quit\n",stdout);
    fputs("Operand count: ",stdout);
    scanf("%d",&opnd_cnt);
    opsmg[0]=(char)opnd_cnt;

        for(int i=0;i<opnd_cnt;i++){
            printf("Operand %d:",i+1);
            scanf("%d",(int*)&opsmg[i*OPSZ+1]);
        }
        fgetc(stdin);
        fputs("Operator: ",stdout);
        scanf("%c",&opsmg[opnd_cnt*OPSZ+1]);
        write(sock,opsmg,opnd_cnt*OPSZ+2);
        read(sock,&result,RLT_SIZE);

        printf("Message from server :%d \n",result);
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