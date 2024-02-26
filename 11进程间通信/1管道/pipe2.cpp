#include <iostream>
#include <unistd.h>
using namespace std;

#define BUF_SIZE 30


int main(){

    int fds[2];
    char str[]="who are you";
    char str2[]="thank you for your message";
    pid_t pid;
    char buf[BUF_SIZE];

    pipe(fds);

    pid=fork();
    if(pid==0){
        write(fds[1],str,sizeof(str));
        sleep(2);
        read(fds[0],buf,BUF_SIZE);
        cout<<"child:"<<buf<<endl;
    }
    else{
        read(fds[0],buf,BUF_SIZE);
        sleep(3);
        write(fds[1],str2,sizeof(str2));
        cout<<"fat:"<<buf<<endl;
    }
    

    return 0;

}