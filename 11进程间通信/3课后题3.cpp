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
void client_rec(int sig);

int main(int argc,char *argv[])
{
    char client[][20]={"one meg","two meg","three meg"};
    char server[][20]={"s one","s two","s three"};
    char message[BUF_SIZE];
    int act_ser[2],act_cln[2];

    struct sigaction act;
    pid_t pid;

    act.sa_handler=client_rec;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    sigaction(SIGCHLD,&act,0);

    pipe(act_ser);
    pipe(act_cln);


    pid=fork();
    if(pid==0){
        for(int i=0;i<3;i++){
        write(act_cln[1],client[i],sizeof(client[i]));
        read(act_ser[0],message,BUF_SIZE);
        printf("client:meg from ser: %s\n",message);
        }
    }
    else{
        for(int i=0;i<3;i++){
        write(act_ser[1],server[i],sizeof(server[i]));
        read(act_cln[0],message,BUF_SIZE);
        printf("server:meg from cln: %s\n",message);
        }
    }
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


