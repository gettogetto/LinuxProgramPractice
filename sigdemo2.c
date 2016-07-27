#include<stdio.h>
#include<signal.h>

int main(){
    signal(SIGINT,SIG_IGN);
    printf("you can not stop me\n");
    while(1){
        sleep(1);
        printf("haha\n");
    }
}
