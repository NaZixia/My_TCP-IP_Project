#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <poll.h>

#define BUF_SIZE 1024
void error_handling(const char *message);
void suc_handling(const char *message);

int main(int argc,char *argv[])
{
    int serv_sock;      //服务端套接字
    int clnt_sock;      //客户端套接字

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_adr_sz;

    char message[BUF_SIZE];
    int str_len;

    //IO复用元素
    struct pollfd pollfd[100];
    nfds_t maxi;
    int timeout;


    if(argc!=2){
        printf("Usage:%s<port>\n",argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    int seteeee=1;
    setsockopt(serv_sock,SOL_SOCKET,SO_REUSEADDR,
    (void*)&seteeee,sizeof(seteeee));

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
	    error_handling("bind()error");

    if(listen(serv_sock,5)==-1)
	    error_handling("listen() error"); 

    pollfd[0].fd=serv_sock;
    pollfd[0].revents=POLLIN;
    timeout=5000;
    maxi=1;

    while(1){
        int fd_num;
        if((fd_num=poll(pollfd,maxi,timeout))==-1)
            break;

        if(fd_num==0){
            continue;
        }
            

        for(int i=0;i<maxi;i++){
            if(pollfd[i].revents&POLLIN)
            {
                if(pollfd[i].fd==serv_sock)
                {
                    clnt_adr_sz=sizeof(clnt_addr);
                    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_adr_sz);
                    pollfd[maxi].fd=clnt_sock;
                    pollfd[maxi].events=POLLIN;
                    maxi++;
                   printf("connect  client%d\n",clnt_sock);
                }
                else
                {
                    str_len=read(pollfd[i].fd,message,BUF_SIZE);
                    if(str_len==0){
                        printf("close clien %d\n",pollfd[i].fd);
                        pollfd[i].fd=-1;
                        pollfd[i].events=POLLIN;
                        close(pollfd[i].fd);
                    }
                    else{
                        write(pollfd[i].fd,message,BUF_SIZE);
                    }
                }
            }
        }
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