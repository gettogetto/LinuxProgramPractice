#include<termio.h>
#include<stdio.h>

#define oops(a,x) perror(a);exit(x);

int main(int argc,char* argv[])
{
    struct termio info;
    if(argc==1) exit(0);

    if(tcgetattr(0,&info)==-1){
        oops("tcgetattr",1);
    }
    if(argv[1][0]=='y'){
        info.c_lflag|=ECHO;
    }else{
        info.c_lflag&=~ECHO;
    }
    if(tcsetattr(0,TCSANOW,&info)==-1){
        oops("tcsetattr",2);
    }

}
