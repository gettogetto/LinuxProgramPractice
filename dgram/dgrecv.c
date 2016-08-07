#include"dgram.h"

#define oops(m,x){perror(m);exit(x);}
void say_who_called(struct sockaddr_in* saddrp);
void reply_to_sender(int sock,char* msg,struct sockaddr_in* addrp,socklen_t len);

int main(int argc,char* argv[])
{
    int port;
    int sock;
    char buf[BUFSIZ];
    size_t msglen;
    struct sockaddr_in saddr;
    socklen_t saddrlen;
    if(argc!=2||(port=atoi(argv[1]))<0){
        fprintf(stderr,"usage:./dgsend portnum\n");
        exit(1);
    }
    sock=make_dgram_server_socket(port);
    if(sock==-1) oops("socket",2);

    saddrlen=sizeof(saddr);
    while((msglen=recvfrom(sock,buf,BUFSIZ,0,(struct sockaddr*)&saddr,&saddrlen))>0){
        buf[msglen]='\0';
        printf("dgrecv:got a message: %s\n",buf);
        say_who_called(&saddr);
        reply_to_sender(sock,buf,&saddr,saddrlen);
    }
    return 0;
}

void say_who_called(struct sockaddr_in *saddrp)
{
    char host[BUFSIZ];
    int port;
    get_internet_address(host,BUFSIZ,&port,saddrp);

    printf("from:%s:%d\n",host,port);
}

void reply_to_sender(int sock,char* msg,struct sockaddr_in* addrp,socklen_t len)
{
    char reply[BUFSIZ];
    sprintf(reply,"Thank for you %d char message\n",strlen(msg));
    sendto(sock,reply,strlen(reply),0,addrp,len);
}
