#include<stdio.h>
#include<signal.h>
#define INPUTLEN 100

int main(){
    struct sigaction newhandler;
    sigset_t blocked;//set of blocked sigs
    void inthandler();
    char x[INPUTLEN];

    newhandler.sa_handler=inthandler;
    newhandler.sa_flags=SA_RESTART;//|SA_RESETHAND;

    sigemptyset(&blocked);
    sigaddset(&blocked,SIGQUIT);
    newhandler.sa_mask=blocked;//sa_mask block signals you like.

    if(sigaction(SIGINT,&newhandler,NULL)==-1){
        perror("sigaction");
    }else{
        while(1){
            fgets(x,INPUTLEN,stdin);
            printf("input:%s",x);
        }
    }
}

void inthandler(int s){
    printf("called with signal %d\n",s);
    sleep(s);
    printf("done handling signal %d\n",s);
}
