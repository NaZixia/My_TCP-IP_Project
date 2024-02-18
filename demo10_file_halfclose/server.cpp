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
    int clnt_sock;      //客户端套接字

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char message[BUFF_SIZE];
    char file_name[30];
    int read_len;

    FILE *file;
    long long FILE_SIZE;

    if(argc!=2){
        printf("Usage:%s<port>\n",argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1){
        error_handling("error serv_sock");
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
	    error_handling("bind()error");

    if(listen(serv_sock,5)==-1)
	    error_handling("listen() error"); 

    clnt_addr_size=sizeof(clnt_addr);
    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
    if(clnt_sock==-1)
        error_handling("accept() error");

    memset(file_name,0,sizeof(file_name));
    read_len=read(clnt_sock,file_name,sizeof(file_name)-1);
    file_name[read_len]=0;

    file=fopen(file_name, "r");
    if (file == NULL){
        printf("File:\t%s Not Found!\n", file_name);
        exit(1);
    }

    while(1){
         memset(message,0,BUFF_SIZE);
         int file_length = fread(message, sizeof(char), BUFF_SIZE, file);
        if(file_length<BUFF_SIZE){
            write(clnt_sock,message,file_length);
            break;
        }
        write(clnt_sock,message,BUFF_SIZE);
    }
    

    shutdown(clnt_sock,SHUT_WR);
    memset(message,0,BUFF_SIZE);
    read(clnt_sock,message,sizeof(message));
    printf("message from client: %s\n",message);
    
    
    fclose(file);
    close(clnt_sock);
    close(serv_sock);
    return 0;

}

void error_handling(const char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}