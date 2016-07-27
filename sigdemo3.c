//several signals

#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#define INPUTLEN 100

int main(int argc,char* argv[]){
    void inthandler(int);
    void quithandler(int);
    char input[INPUTLEN];
    int nchars;

    signal(SIGINT,inthandler);
    signal(SIGQUIT,quithandler);

    do{
        printf("\nType a message\n");
        nchars=read(0,input,(INPUTLEN-1));
        if(nchars==-1){
            perror("read returned an error\n");
        }else{
            input[nchars]='\0';
            printf("you typed:%s",input);
        }
    }while(strncmp(input,"quit",4)!=0);

}

void inthandler(int s){
    printf("\nreceived signal %d.. waiting\n",s);
    sleep(2);
    printf("leaving inhandler\n");
}

void quithandler(int s){
    printf("\nreceived signal %d.. waiting\n",s);
    sleep(3);
    printf("leaving quithandler\n");
}
