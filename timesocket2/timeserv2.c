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
void process_request(int fd){
    time_t now;
    char* cp;
    time(&now);
    cp=ctime(&now);
    write(fd,cp,strlen(cp));
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
    while(1){
        fd=accept(sock,NULL,NULL);
        if(fd==-1) exit(1);
        process_request(fd);
        close(fd);
    }

}
