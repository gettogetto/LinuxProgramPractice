#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#define BUFFSIZE 4096
#define COPYMODE 0644//110 100 100

void oops(char*,char*);

int main(int argc, char *argv[])
{
    int in_fd,out_fd,n_chars;
    char buf[BUFFSIZE];

    if(argc!=3){
        fprintf(stderr,"useage:%s source destination\n",*argv);
        exit(1);
    }

    if((in_fd=open(argv[1],O_RDONLY))==-1){
        oops("can not open",argv[1]);
    }
    if((out_fd=creat(argv[2],COPYMODE))==-1){
        oops("can not creat",argv[2]);
    }
    while((n_chars=read(in_fd,buf,BUFFSIZE))>0){
        if((write(out_fd,buf,n_chars))!=n_chars){//after call write(),must check the written char numbers
            oops("write error to",argv[2]);
        }
    }
    if(n_chars==-1){
        oops("read error from",argv[1]);
    }

    if(close(in_fd)==-1||close(out_fd)==-1){
        oops("error closing files","");
    }


    return 0;
}
void oops(char *s1, char *s2){
    fprintf(stderr,"Error:%s",s1);
    perror(s2);
    exit(1);
}
