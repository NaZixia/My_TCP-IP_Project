#include <iostream>
#include <unistd.h>

using namespace std;

int main(){
    pid_t pid=fork();
    if(pid==0)
        cout<<"我是子进程"<<endl;
    else
    {
        cout<<"子进程ID:"<<pid<<endl;
        sleep(30);
    }

    if(pid==0)
        cout<<"子进程结束"<<endl;
    else    
        cout<<"父进程结束"<<endl;

    return 0;

}