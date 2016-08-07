
#include"dgram.h"

#define HOSTLEN 256

int make_internet_address(char* hostname,int portnum,struct sockaddr_in* addrp);
int make_dgram_server_socket(int portnum)
{
    struct sockaddr_in saddr;
    char hostname[HOSTLEN];
    int sock_id;
    sock_id=socket(AF_INET,SOCK_DGRAM,0);
    if(sock_id==-1) return -1;

    gethostname(hostname,HOSTLEN);
    make_internet_address(hostname,portnum,&saddr);
    if(bind(sock_id,(struct sockaddr*)&saddr,sizeof(saddr))!=0) return -1;
    return sock_id;
}
int make_internet_address(char* hostname,int portnum,struct sockaddr_in* addrp){
    struct hostent* hp;
    hp=gethostbyname(hostname);
    if(hp==NULL) return -1;
    bcopy((void*)hp->h_addr_list[0],(void*)&addrp->sin_addr,hp->h_length);
    addrp->sin_family=AF_INET;
    addrp->sin_port=htons(portnum);
    return 0;
}
int get_internet_address(char* host,int len,int *portp,struct sockaddr_in* addrp)
{
    strncpy(host,inet_ntoa(addrp->sin_addr),len);
    *portp=ntohs(addrp->sin_port);
    return 0;
}
int make_dgram_client_socket()
{
    return socket(AF_INET,SOCK_DGRAM,0);
}
