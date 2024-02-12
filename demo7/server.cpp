#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(const char *message);
void suc_handling(const char *message);
long long end;
void compute(int *arr,int n,char Operator);

int main(int argc,char *argv[])
{
    int serv_sock;      //服务端套接字
    int clnt_sock;      //客户端套接字

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_adr_sz;

    char message[BUF_SIZE];
    int str_len,i;
    int number[256];
    char Operator;

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

    clnt_adr_sz=sizeof(clnt_addr);
    for(int n=0;n<5;n++){
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_adr_sz);
        if(clnt_sock==-1)
            error_handling("accept() error");
        else
            printf("Connect client %d \n",n+1);

        str_len=read(clnt_sock,message,BUF_SIZE);
        if(str_len==-1)
            error_handling("error read");
            
        int nub=atoi(message);
        for(i=0;i<nub;i++){
            str_len=read(clnt_sock,message,BUF_SIZE);
            if(str_len==-1)
                error_handling("error read");
            number[i]=atoi(message);
        }
        str_len=read(clnt_sock,message,BUF_SIZE);
        if(str_len==-1)
            error_handling("error read");
        Operator=message[0];

        compute(number,nub,Operator);
        //memcpy(message,0,sizeof(message));
        message[0]=end;
        printf("nub= %lld",end);
        write(clnt_sock,message,str_len);   //back call

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

void compute(int *arr,int n,char Operator){

    if(Operator=='*'){
        end=1;
        for(int i=0;i<n;i++){
            end*=arr[i];
        }
    }
    else if(Operator=='/'){
        end=arr[0];
        for(int i=1;i<n;i++){
            end/=arr[i];
        }
    }
    else if(Operator=='+'){
        end=0;
        for(int i=0;i<n;i++){
            end+=arr[i];
        }
    }
    else if(Operator=='-'){
        end=arr[0];
        for(int i=1;i<n;i++){
            end-=arr[i];
        }
    }
    else{
        error_handling("error to Operator");
    }
    return ;
}