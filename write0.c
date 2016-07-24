#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
//communication between two tty
int main(int argc,char* argv[])
{
    int fd;
    char buf[BUFSIZ];
    if(argc!=2){
        fprintf(stderr,"useage:write0 ttyname\n");
        exit(1);
    }

    fd=open(argv[1],O_WRONLY);
    if(fd==-1){
        perror(argv[1]);
        exit(1);
    }
    while(fgets(buf,BUFSIZ,stdin)!=NULL){    //while(gets(buf)!=NULL){
        //fgets will end with '\n',while gets not.
        if(write(fd,buf,strlen(buf))==-1){
            break;
        }
    }
    close(fd);
}
