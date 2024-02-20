#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

void error_handling(const char *message);

int main(int argc,char *argv[])
{
    int sock;
    int snd_buf,rcv_buf,state;
    socklen_t len;
    
    sock=socket(PF_INET,SOCK_STREAM,0);
    len=sizeof(sock);
    state=getsockopt(sock,SOL_SOCKET,SO_SNDBUF,(void*)&snd_buf,&len);
    if(state)
        error_handling("error getsockopt");

    len=sizeof(sock);
    state=getsockopt(sock,SOL_SOCKET,SO_RCVBUF,(void*)&rcv_buf,&len);
    if(state)
        error_handling("error getsockopt");
    
    printf("Send buf size: %d\n",snd_buf);
    printf("Recv buf size: %d\n",rcv_buf);
    
    return 0;
}

void error_handling(const char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}