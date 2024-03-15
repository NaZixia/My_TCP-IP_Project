#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

#define BUF_SIZE 30
void error_handling(const char *message);
void suc_handling(const char *message);
void client_server(int sig);

int main(int argc,char *argv[])
{
    int serv_sock;      //服务端套接字
    int clnt_sock;      //客户端套接字

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_adr_sz;

    char message[BUF_SIZE];
    int str_len,state;

    pid_t pid;
    struct sigaction act;

    if(argc!=2){
        printf("Usage:%s<port>\n",argv[0]);
        exit(1);
    }

    //完成服务 结构体初始化
    act.sa_handler=client_server;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    state=sigaction(SIGCHLD,&act,0);

    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1){
        error_handling("error serv_sock creat");
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
	    error_handling("bind()error");

    if(listen(serv_sock,5)==-1)
	    error_handling("listen() error"); 

    int n=0;
    while(1){
        clnt_adr_sz=sizeof(clnt_addr);
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_adr_sz);
        if(clnt_sock==-1)
            continue;
        else{
            printf("new Connect client %d \n",n+1);
            n++;
        }
        pid=fork();   
        if(pid==-1){
            close(clnt_sock);
            continue;
        }
        if(pid==0){
            close(serv_sock);
            while((str_len=read(clnt_sock,message,BUF_SIZE))!=0)
                write(clnt_sock,message,str_len);   //back call
            close(clnt_sock);
            printf("client disconnect\n");
            return 0;
        }
        close(clnt_sock);
    }
    
    close(serv_sock);
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

void client_server(int sig){
    pid_t pid;
    int status;
    pid=waitpid(-1,&status,WNOHANG);
    printf("remove client:%d server succes\n",pid);
}
