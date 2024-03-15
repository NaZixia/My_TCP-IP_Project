#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;


void keycontrol(int sig){
    if(sig==SIGINT){
        cout<<"CTRL + C pressed"<<endl;
        cout<<"Y to exit"<<endl;
        char choose;
        cin>>choose;
        if(choose=='Y'){
            exit(1);
        }
        }
}


int main(){
    struct sigaction s2,time;

    s2.sa_handler=keycontrol;
    sigemptyset(&s2.sa_mask);
    s2.sa_flags=0;
    sigaction(SIGINT,&s2,0);

    
    while(1){
        sleep(1);
        cout<<"print message"<<endl;
    }


    return 0;

}