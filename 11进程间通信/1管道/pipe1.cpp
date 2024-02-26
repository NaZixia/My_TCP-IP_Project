#include <iostream>
#include <unistd.h>
using namespace std;

#define BUF_SIZE 30


int main(){

    int fds[2];
    char str[]="who are you";
    pid_t pid;
    char buf[BUF_SIZE];

    pipe(fds);

    pid=fork();
    if(pid==0){
        write(fds[1],str,sizeof(str));
    }
    else{
        read(fds[0],buf,BUF_SIZE);
        cout<<buf<<endl;
    }
    

    return 0;

}