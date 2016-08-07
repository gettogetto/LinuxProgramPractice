#include"dgram.h"
#define oops(m,x){perror(m);exit(x);}
void display_received_from_server(int sock,char* buf,int bufsize,struct sockaddr_in* saddrp){
    recvfrom(sock,buf,bufsize,0,saddrp,sizeof(*saddrp));
    printf("received %s from %s:%d\n",buf,inet_ntoa(saddrp->sin_addr),ntohs(saddrp->sin_port));

}

int main(int argc, char *argv[])
{
    int sock;
    char *msg;
    struct sockaddr_in saddr;

    if(argc!=4){
        fprintf(stderr,"usage:./dgsend host port 'message' \n");
        exit(1);
    }
    sock=make_dgram_client_socket();
    if(sock==-1) oops("socket",1);

    msg=argv[3];
    make_internet_address(argv[1],atoi(argv[2]),&saddr);
    if(sendto(sock,msg,strlen(msg),0,(struct sockaddr*)&saddr,sizeof(saddr))==-1)
    {
        oops("send",1);
    }
    char recv[BUFSIZ];

    display_received_from_server(sock,recv,BUFSIZ,&saddr);

    return 0;
}
