#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>//internet adress family
#include<netdb.h>//for hostent
#include"socklib.h"
#define HOSTLEN 256
#define BACKLOG 1
#define oops(msg){perror(msg);return(-1);}

int make_server_socket_q(int portnum,int backlog){
    int sock_id;
    struct sockaddr_in saddr;
    struct hostent *hp;
    char hostname[HOSTLEN];
    sock_id=socket(AF_INET,SOCK_STREAM,0);
    if(sock_id==-1) oops("socket");

    int ret=0;
    ret=gethostname(hostname,HOSTLEN);
    if(ret==-1) oops("gethostname");

    hp=gethostbyname(hostname);

    if(hp==NULL) oops("gethostbyname");

    bzero((void*)&saddr,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(portnum);
    bcopy(hp->h_addr_list[0],(void*)&saddr.sin_addr,hp->h_length);

    char str[32];
    inet_ntop(saddr.sin_family,&saddr.sin_addr,str,sizeof(str));
    printf("%s\n",str);/////////////////////////


    ret=bind(sock_id,(struct sockaddr*)&saddr,sizeof(saddr));
    if(ret!=0) oops("bind");

    ret=listen(sock_id,backlog);
    if(ret!=0) oops("listen");
    return sock_id;
}

int make_server_socket(int portnum){
    return make_server_socket_q(portnum,BACKLOG);
}

int connect_to_server(char* host,int portnum)
{
    int sock;
    struct sockaddr_in saddr;
    struct hostent* hp;
   // char hostname[HOSTLEN];

    hp=gethostbyname(host);
    if(hp==NULL) oops("gethostbyname");

    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock==-1) oops("socket");

    bzero((void*)&saddr,sizeof(saddr));

    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(portnum);

    bcopy((void*)hp->h_addr_list[0],(void*)&saddr.sin_addr,hp->h_length);

    int ret=connect(sock,(struct sockaddr*)&saddr,sizeof(saddr));
    if(ret!=0) oops("connect");

    return sock;

}
