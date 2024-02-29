#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

#define BUF_SIZE 30

int main(int argc,char *argv[])
{
    char buf[BUF_SIZE];
    fd_set reads,temps;
    int result,str_len;
    struct timeval  timeout;
    FD_ZERO(&reads);
    FD_SET(0,&reads);

    while(1){
        temps=reads;
        timeout.tv_sec=5;
        timeout.tv_usec=0;
        result=select(1,&temps,0,0,&timeout);
        if(result==-1){
            fputs("error select\n",stdout);
            break;
        }
        else if(result==0){
            fputs("time out\n",stdout);
            break;
        }
        else{  
            if(FD_ISSET(0,&temps)){
                str_len=read(0,buf,BUF_SIZE);
                buf[str_len]=0;
                printf("message: %s",buf);
            }
        }
    }

    return 0;
}


