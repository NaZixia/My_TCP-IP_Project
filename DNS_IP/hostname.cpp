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
    int i;
    struct hostent *host;
    
    if(argc!=2){
        printf("Usage:%s <IP>\n",argv[0]);
        exit(1);
    }

    host=gethostbyname(argv[1]);

    printf("host of_name: %s\n",host->h_name);
    for(i=0;host->h_aliases[i];i++)
        printf("The %d host other name: %s\n",
            i+1,host->h_aliases[i]);
    printf("addrType: %s\n",
    (host->h_addrtype==AF_INET)?"AF_INET":"ipv6");
    //for(i=0;host->h_addr_list[i];i++)
       // printf("The %d other IP: s \n",
            //i+1,inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

        //与上方相同,更容易理解
    for (int i = 0; host->h_addr_list[i] != NULL; i++) {
        struct in_addr addr;
        memcpy(&addr, host->h_addr_list[i], sizeof(struct in_addr));
        printf("The %d other IP: %s \n", i+1, inet_ntoa(addr));
    }
    return 0;
}

void error_handling(const char *message){
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}