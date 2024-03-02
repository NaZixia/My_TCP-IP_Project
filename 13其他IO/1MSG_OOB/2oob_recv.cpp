#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define BUF_SIZE 100
void error_handling(const char *message);
void suc_handling(const char *message);
void urg_handler(int sig);

int acpt_sock;
int recv_sock;

int main(int argc,char *argv[])
{
    struct sockaddr_in recv_addr;
    struct sockaddr_in serv_addr;
    socklen_t serv_adr_sz;

    char message[BUF_SIZE];
    int str_len,state;

    struct sigaction act;

    if(argc!=2){
        printf("Usage:%s<port>\n",argv[0]);
        exit(1);
    }

    act.sa_handler=urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;

    acpt_sock=socket(PF_INET,SOCK_STREAM,0);
    if(acpt_sock==-1){
        error_handling("error serv_sock creat");
    }

    memset(&recv_addr,0,sizeof(recv_addr));
    recv_addr.sin_family=AF_INET;
    recv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    recv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(acpt_sock,(struct sockaddr*)&recv_addr,sizeof(recv_addr))==-1)
	    error_handling("bind()error");

    if(listen(acpt_sock,5)==-1)
	    error_handling("listen() error"); 

    serv_adr_sz=sizeof(serv_addr);
    recv_sock=accept(acpt_sock,(struct sockaddr*)&serv_addr,&serv_adr_sz);

    fcntl(recv_sock,F_SETOWN,getpid());
    state=sigaction(SIGURG,&act,0);

    while((str_len=recv(recv_sock,message,sizeof(message),0))!=0){
        if(str_len==-1)
            continue;
        message[str_len]=0;
        puts(message);
    }

    close(recv_sock);
    close(acpt_sock);
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

void urg_handler(int sig){
    int str_len;
    char buf[BUF_SIZE];
    str_len=recv(recv_sock,buf,BUF_SIZE-1,MSG_OOB);
    buf[str_len]=0;
    puts(buf);
}

