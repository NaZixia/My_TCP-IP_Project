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
    int tcp_sock,udp_sock;
    int sock_type;
    socklen_t optlen;
    int stare;
    
    tcp_sock=socket(PF_INET,SOCK_STREAM,0);
    udp_sock=socket(PF_INET,SOCK_DGRAM,0);
    optlen=sizeof(sock_type);
    printf("SOCK_STREAM: %d \n",SOCK_STREAM);
    printf("SOCK_DGRAM : %d \n",SOCK_DGRAM);
    
    stare=getsockopt(tcp_sock,SOL_SOCKET,SO_TYPE,(void*)&sock_type,&optlen);
    if(stare)
        error_handling("error get tcp_sock opt");
    printf("socket type one : %d  \n",sock_type);

    stare=getsockopt(udp_sock,SOL_SOCKET,SO_TYPE,(void*)&sock_type,&optlen);
    if(stare)
        error_handling("error get udp_sock opt");
    printf("socket type two : %d  \n",sock_type);
    
    return 0;
}

void error_handling(const char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}