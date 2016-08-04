#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>

#include<netinet/in.h>
#include<netdb.h>
#include<time.h>

#include<string.h>

#define PORTNUM 13000
#define HOSTLEN 256
#define oops(msg) {perror(msg);exit(1);}
#define CHECK(msg,ret){if(ret==-1){oops(msg);}}
int main(int argc,char* argv[]){
    struct sockaddr_in saddr;
    struct hostent *hp;
    char hostname[HOSTLEN];
    int sock_fd,sock_id;
    FILE* sock_fp;

    time_t thetime;
/*****/
    sock_id=socket(AF_INET,SOCK_STREAM,0);
    CHECK("socket",sock_id);
    bzero((void*)&saddr,sizeof(saddr));

    gethostname(hostname,HOSTLEN);
    hp=gethostbyname(hostname);

    bcopy((void*)hp->h_addr,(void*)&saddr.sin_addr,hp->h_length);

    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(PORTNUM);
/*****/
    CHECK("bind",bind(sock_id,(const struct sockaddr*)&saddr,sizeof(saddr)));//
/*****/
    CHECK("listen",listen(sock_id,1));//1:valid number of connections

    while(1){
/*****/

        sock_fd=accept(sock_id,NULL,NULL);
        CHECK("accept",sock_fd)
        printf("got a call\n");

        sock_fp=fdopen(sock_fd,"w");
        if(sock_fp==NULL){
            oops("fdopen");
        }
        thetime=time(NULL);
        fprintf(sock_fp,"the time is..");
        fprintf(sock_fp,"%s",ctime(&thetime));
        /*use write*/
        /*char message[BUFSIZ];
        strcpy(message,"the time is...\n");
        strcat(message,ctime(&thetime));
        write(sock_fd,message,strlen(message)-1);*/
        fclose(sock_fp);
    }



}
