#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

using namespace std;

void read_childproc(int sig){
    int status;
    pid_t id=waitpid(-1,&status,WNOHANG);
    if(WIFEXITED(status)){
        cout<<"Remove proc id: "<<id<<endl;
        cout<<"Child send: "<<WIFEXITED(status)<<endl;
    }
}

int main(){
    struct sigaction act;
    pid_t pid;

    act.sa_handler=read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    sigaction(SIGCHLD,&act,0);

    pid=fork();

    if(pid==0){
        cout<<"hi,i am child"<<endl;
        sleep(5);
        return 12;
    }

    cout<<"child id :"<<pid<<endl;
    pid=fork();
    if(pid==0){
        cout<<"hello,im child "<<endl;
        sleep(5);
        exit(24);
    }

    cout<<"child id :"<<pid<<endl;
    for(int i=0;i<3;i++){
        cout<<"wait..."<<endl;
        sleep(5);
    }

    return 0;

}