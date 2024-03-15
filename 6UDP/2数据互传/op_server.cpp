#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFF_SIZE 1024

void error_handling(const char *message);

int main(int argc,char *argv[])
{

    int serv_sock;      //服务端套接字

    struct sockaddr_in clnt_addr;
    socklen_t clnt_adr_sz;

    char message[BUFF_SIZE];
    int str_len;

    if(argc!=2){
        printf("Usage:%s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET,SOCK_DGRAM,0);
    if(serv_sock==-1){
        error_handling("error UDP serv_sock cretion");
    }

    memset(&clnt_addr,0,sizeof(clnt_addr));
    clnt_addr.sin_family=AF_INET;
    clnt_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    clnt_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&clnt_addr,sizeof(clnt_addr))==-1)
	    error_handling("bind()error");

    clnt_adr_sz=sizeof(clnt_addr);
    while(1){
        memset(&message,0,BUFF_SIZE);
        str_len=recvfrom(serv_sock,message,BUFF_SIZE,0,
            (struct sockaddr*)&clnt_addr,&clnt_adr_sz);
        message[str_len]=0;
        printf("Message from client :%s \n",message);

        memset(&message,0,BUFF_SIZE);
        fputs("Insert message(Q to quit): ",stdout);
        fgets(message,sizeof(message),stdin);
        if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
            break;
        sendto(serv_sock,message,sizeof(message),0,
            (struct sockaddr*)&clnt_addr,clnt_adr_sz);
        
    }
    close(serv_sock);
    return 0;

}

void error_handling(const char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}
