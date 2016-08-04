#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>//internet adress family
#include<netdb.h>//for hostent
#include"socklib.h"
#include<time.h>
#include<signal.h>
void process_request(int fd){
    if(fork()==0){
        dup2(fd,1);
        close(fd);
        execlp("date","date",NULL);
    }
}

void child_waiter(int signum){
    while(waitpid(-1,NULL,WNOHANG)>0);
}

int main(int argc,char* argv[]){
    int sock,fd;
    if(argc!=2){
        fprintf(stderr,"./timeclnt2 portnum");
        exit(1);
    }
    int portnum=atoi(argv[1]);
    sock=make_server_socket(portnum);
    if(sock==-1) exit(1);

    signal(SIGCHLD,child_waiter);
    while(1){
        fd=accept(sock,NULL,NULL);
        if(fd==-1) exit(1);
        process_request(fd);
        close(fd);
    }

}
