#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 100
void error_handling(const char *message);
void suc_handling(const char *message);



int main(int argc,char *argv[])
{
    int acpt_sock, recv_sock;
    struct sockaddr_in recv_addr,acpt_addr;
    socklen_t recv_adr_sz;

    char message[BUF_SIZE];
    int str_len,state;

    if(argc!=2){
        printf("Usage:%s<port>\n",argv[0]);
        exit(1);
    }

    acpt_sock=socket(PF_INET,SOCK_STREAM,0);
    if(acpt_sock==-1){
        error_handling("error serv_sock creat");
    }

    memset(&acpt_addr,0,sizeof(acpt_addr));
    acpt_addr.sin_family=AF_INET;
    acpt_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    acpt_addr.sin_port=htons(atoi(argv[1]));

    if(bind(acpt_sock,(struct sockaddr*)&acpt_addr,sizeof(acpt_addr))==-1)
	    error_handling("bind()error");

    if(listen(acpt_sock,5)==-1)
	    error_handling("listen() error"); 

    recv_adr_sz=sizeof(recv_addr);
    recv_sock=accept(acpt_sock,(struct sockaddr*)&recv_addr,&recv_adr_sz);

    while(1){
        str_len=recv(recv_sock,message,sizeof(message)-1,MSG_PEEK);
        if(str_len>0)
            break;
    }
    message[str_len]=0;
    printf("len: %d meg:%s\n",str_len,message);

    str_len=recv(recv_sock,message,sizeof(message)-1,0);
    message[str_len]=0;
    printf("m:%s",message);
    

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



