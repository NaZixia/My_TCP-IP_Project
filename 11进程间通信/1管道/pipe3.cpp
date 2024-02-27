#include <iostream>
#include <unistd.h>
using namespace std;

#define BUF_SIZE 30


int main(){

    int fds1[2],fds2[2];
    char str[]="who are you";
    char str2[]="thank you for your message";
    pid_t pid;
    char buf[BUF_SIZE];

    pipe(fds1);
    pipe(fds2);

    pid=fork();
    if(pid==0){
        write(fds2[1],str,sizeof(str));
        read(fds1[0],buf,BUF_SIZE);
        cout<<"child:"<<buf<<endl;
    }
    else{
        read(fds2[0],buf,BUF_SIZE);
        write(fds1[1],str2,sizeof(str2));
        cout<<"fat:"<<buf<<endl;
    }
    

    return 0;

}