#include<stdio.h>
#include<signal.h>
#include<unistd.h>
int main(){
    void wakeup(int);
    printf("about to sleep 4s\n");
    signal(SIGALRM,wakeup);
    alarm(4);//send a SIGALARM signal to the process in 4 seconds
    pause();//hangup the process until a signal arrived
    printf("I waked up\n");
}

void wakeup(int signum){
    printf("Alarm received from kernel\n");
}
