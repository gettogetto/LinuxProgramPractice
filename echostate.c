#include<stdio.h>
#include<termio.h>
int main(){
    struct termio info;
    int rv;
    rv=tcgetattr(0,&info);//0 is fd of stdin,get values from driver
    if(rv==-1){
        perror("tcgetattr");
        exit(1);
    }
    if(info.c_lflag&ECHO) printf("echo is on\n");
    else printf("echo is off\n");
    return 0;
}
