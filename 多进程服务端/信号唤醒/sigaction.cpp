#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

void timeout(int sig){
    if(sig==SIGALRM)
        cout<<"Time out!"<<endl;
    alarm(3);
}

void keycontrol(int sig){
    if(sig==SIGINT)
        cout<<"CTRL + C pressed"<<endl;
}

int main(){
    struct sigaction s1,s2;
    
    s1.sa_handler=timeout;
    sigemptyset(&s1.sa_mask);
    s1.sa_flags=0;
    sigaction(SIGALRM,&s1,0);

    s2.sa_handler=keycontrol;
    sigemptyset(&s1.sa_mask);
    s2.sa_flags=0;
    sigaction(SIGINT,&s2,0);

    alarm(2);

    for(int i=0;i<3;i++){
        cout<<"wait.."<<endl;
        sleep(100);
    } 

    return 0;

}