#include<string.h>
#include<stdio.h>
#include<unistd.h>

#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdlib.h>

#define PORTNUM 15100
#define HOSTLEN 256
#define oops(msg){perror(msg);exit(1);}

void sanitize(char* str){
    char* src,*dest;
    for(src=dest=str;*src;src++){
        if(*src=='/'||isalnum(*src))
            *dest++=*src;
    }
    *dest='\0';
}

int main(int argc,char*argv[]){
    int sock_id,sock_fd;
    FILE* sock_fpi,*sock_fpo,*pipe_fp;
    struct sockaddr_in saddr;
    struct hostent* hp;

    char hostname[HOSTLEN];
    if(gethostname(hostname,HOSTLEN)==-1) oops("gethostname");

    hp=gethostbyname(hostname);
    if(hp==NULL) oops("gethostbyname");

    sock_id=socket(AF_INET,SOCK_STREAM,0);
    if(sock_id==-1) oops("socket");

    bzero((void*)&saddr,sizeof(saddr));
    bcopy((void*)hp->h_addr,(void*)&saddr.sin_addr,hp->h_length);
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(PORTNUM);
    if(bind(sock_id,(struct sockaddr*)&saddr,sizeof(saddr))!=0) oops("bind");

    if(listen(sock_id,1)!=0) oops("listen");



    char dirname[BUFSIZ];
    char command[BUFSIZ];
    while(1){
        sock_fd=accept(sock_id,NULL,NULL);

        if(sock_fd==-1) oops("accept");

        sock_fpi=fdopen(sock_fd,"r");
        if(sock_fpi==NULL ) oops("fdopen read");

        if(fgets(dirname,BUFSIZ-5,sock_fpi)==NULL) oops("fgets");

        sanitize(dirname);

        if((sock_fpo=fdopen(sock_fd,"w"))==NULL) oops("fdopen write");

        sprintf(command,"ls %s",dirname);

        if((pipe_fp=popen(command,"r"))==NULL) oops("popen");
        char c;
        while((c=getc(pipe_fp))!=EOF){
            putc(c,sock_fpo);
        }

        pclose(pipe_fp);

        fclose(sock_fpo);
        fclose(sock_fpi);


    }



}
