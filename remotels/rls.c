#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#define oops(msg){perror(msg);exit(1);}
#define PORTNUM 15100
int main(int argc,char* argv[]){
    struct sockaddr_in saddr;
    struct hostent* hp;
    int sock_id;
    hp=gethostbyname(argv[1]);
    if(hp==NULL){
        oops("gethostbyname");
    }
    sock_id=socket(AF_INET,SOCK_STREAM,0);
    if(sock_id==-1) oops("socket");

    bzero((void*)&saddr,sizeof(saddr));

    bcopy(hp->h_addr,(struct sockaddr*)&saddr.sin_addr,hp->h_length);

    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(PORTNUM);

    if( connect(sock_id,(struct sockaddr*)&saddr,sizeof(saddr))!=0 ) oops("connect");


    if(write(sock_id,argv[2],strlen(argv[2]))==-1) oops("write");
    if(write(sock_id,"\n",1)==-1) oops("write");
    int n_read;
    char buffer[BUFSIZ];
    while((n_read=read(sock_id,buffer,BUFSIZ))>0){
        if(write(STDOUT_FILENO,buffer,n_read)==-1){
            oops("write");
        }


    }
    close(sock_id);



}
