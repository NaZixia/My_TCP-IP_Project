#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
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
    int str_len,i;

    //IO复用元素
    struct timeval timeout;
    fd_set reads,cpy_reads;
    int fd_max,fd_num;

    if(argc!=2){
        printf("Usage:%s<port>\n",argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
	    error_handling("bind()error");

    if(listen(serv_sock,5)==-1)
	    error_handling("listen() error"); 

    FD_ZERO(&reads);
    FD_SET(serv_sock,&reads);
    fd_max=serv_sock;

    while(1){
        cpy_reads=reads;
        timeout.tv_sec=5;
        timeout.tv_usec=0;

        if((fd_num=select(fd_max+1,&cpy_reads,0,0,&timeout))==-1)
            break;
        if(fd_num==0)
            continue;
        fd_num=0;
        for(i=0;i<fd_max+1;i++){
            if(FD_ISSET(i,&cpy_reads))
            { 
                if(i==serv_sock)
                {
                    clnt_adr_sz=sizeof(clnt_addr);
                    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_adr_sz);
                    FD_SET(clnt_sock,&reads);
                    if(fd_max<clnt_sock)
                        fd_max=clnt_sock;
                    int dd=clnt_sock;
                    printf("connect %d client\n",dd);
                }
                else
                {
                    str_len=read(i,message,BUF_SIZE);
                    if(str_len==0){
                        FD_CLR(i,&reads);
                        close(i);
                        printf("close clien %d\n",i);
                    }
                    else{
                        write(i,message,str_len);
                    }
                }
            }
        }
        close(serv_sock);
        return 0;
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