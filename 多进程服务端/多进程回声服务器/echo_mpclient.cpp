#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define BUF_SIZE 1024
void error_handling(const char *message);
void suc_handling(const char *message);
void client_rec(int sig);
void write_routine(int sock,char *buf);
void read_routine(int sock,char *buf);

int main(int argc,char *argv[])
{
    int sock;
    struct sockaddr_in serv_adr;
    char message[BUF_SIZE];

    int recv_len,recv_cnt; 

    struct sigaction act;
    pid_t pid;

    if(argc!=2){
        printf("Usage:%s<IP> <port>\n",argv[0]);
        exit(1);
    }

    act.sa_handler=client_rec;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    sigaction(SIGCHLD,&act,0);


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

    pid=fork();
    if(pid==0)
        read_routine(sock,message);
    else
        write_routine(sock,message);
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

void client_rec(int sig){
    pid_t pid;
    int status;
    pid=waitpid(-1,&status,WNOHANG);
    printf("remove client:%d server succes\n",pid);
}



void write_routine(int sock,char *buf){
    while(1){
        fgets(buf,sizeof(buf),stdin);
        if(!strcmp(buf,"q\n")||!strcmp(buf,"Q\n")){
            shutdown(sock,SHUT_WR);
            return;
        }
        write(sock,buf,strlen(buf));
    }
}

void read_routine(int sock,char *buf){
    while(1){
            int recv_len=read(sock,buf,sizeof(buf));
            if(recv_len==0)
                return;
            buf[recv_len]=0;
            printf("Message from server :%s \n",buf);
        }
}