#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>//internet adress family
#include<netdb.h>//for hostent
#include"socklib.h"

void talk_with_server(int fd){
    char buf[BUFSIZ];
    int n=read(fd,buf,BUFSIZ);
    write(STDOUT_FILENO,buf,n);
}

int main(int argc,char* argv[]){
    int fd;
    if(argc!=3){
        fprintf(stderr,"./timeclnt2 hostname portnum");
        exit(1);
    }
    char* hostname=argv[1];
    int portnum=atoi(argv[2]);
    fd=connect_to_server(hostname,portnum);
    if(fd==-1) exit(1);

    talk_with_server(fd);
    close(fd);
}
