/*a multi thread webserver
 * usage:./webserv2 portnum
 * run in the current directory
 * support GET only
 *
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<dirent.h>
#include<time.h>
#include<pthread.h>
#include<string.h>
#include<stdbool.h>
#include"socklib.h"
#define oops(msg,num){perror(msg);exit(num);}


time_t server_started;
int server_bytes_sent;
int server_requests;//record requests



void *handle_call(void*);
void setup(pthread_attr_t*);
void skip_rest_of_header(FILE* fpin);
void process_rq(char* request,int fd);
void sanitize(char* arg);
void not_implemented(int);
bool built_in(char* arg,int fd);
bool not_exist(char*arg,int fd);

void do_404(char* arg,int fd);
bool isadir(char* arg);
void do_ls(char* arg,int fd);
void do_cat(char* arg,int fd);

char *file_type(char* arg);

int http_reply(int fd,FILE** fpp,int statcode,char *msg,char* type,char*content);


int main(int argc,char* argv[])
{
    int sock,fd;
    int* fdptr;
    pthread_t worker;
    pthread_attr_t attr;
    if(argc!=2){
        fprintf(stderr,"usage:./webserv2 portnum\n");
        exit(1);
    }
    sock=make_server_socket(atoi(argv[1]));
    if(sock==-1) oops("make_server_socket",1);
/*initialize*/
    setup(&attr);
/*main loop*/
    while(1)
    {
        fd=accept(sock,NULL,NULL);
        if(fd==-1) oops("accept",1);
        server_requests++;
        fdptr=malloc(sizeof(int));
        *fdptr=fd;
        pthread_create(&worker,&attr,handle_call,(void*)fdptr);
    }
}

void setup(pthread_attr_t *attrp)
{
    pthread_attr_init(attrp);
    pthread_attr_setdetachstate(attrp,PTHREAD_CREATE_DETACHED);

    time(&server_started);
    server_requests=0;
    server_bytes_sent=0;

}

void* handle_call(void * fdptr)
{
    int fd=*(int*)fdptr;
    free(fdptr);

    FILE* fpin;
    char request[BUFSIZ];

    fpin=fdopen(fd,"r");
    fgets(request,BUFSIZ,fpin);
    printf("got a call on %d:request=%s",fd,request);

    skip_rest_of_header(fpin);
    process_rq(request,fd);
    fclose(fpin);
}


/*skip over all request info until a crnl is seen*/
void skip_rest_of_header(FILE *fpin)
{
    char buf[BUFSIZ];
    while(fgets(buf,BUFSIZ,fpin)!=NULL&&strcmp(buf,"\r\n")!=0);
}

/*do what the request asks for and write reply to fd.
 * handle a request in a new thread
 * request is http command:"GET /fpp/bar.html HTTP/1.0" */
void process_rq(char *request, int fd)
{
    char cmd[BUFSIZ];
    char arg[BUFSIZ];
    if(sscanf(request,"%s%s",cmd,arg)!=2){
        return;
    }
    sanitize(arg);
    if(strcmp(cmd,"GET")!=0){
        not_implemented(fd);
    }else if(built_in(arg,fd)){
        ;
    }else if(not_exist(arg,fd)){
        do_404(arg,fd);
    }else if(isadir(arg)){
        do_ls(arg,fd);
    }else{
        do_cat(arg,fd);
    }

}


void sanitize(char *arg)
{
    char* src,*dest;
    src=dest=arg;
    while(*src){
        if(strncmp(src,"/../",4)==0){
            src+=3;
        }else if(strncmp(src,"/",2)==0){
            src++;
        }else{
            *dest++=*src++;
        }

    }
    *dest='\0';
    if(*arg=='/') strcpy(arg,arg+1);
    if(arg[0]=='\0'||strcmp(arg,"./")==0||strcmp(arg,"./..")==0){
        strcpy(arg,".");
    }


}

bool built_in(char *arg, int fd)
{
    FILE* fp;
    if(strcmp(arg,"status")!=0)
        return 0;
    server_bytes_sent+=http_reply(fd,&fp,200,"OK","text/plain",NULL);

    fprintf(fp,"Server started: %s",ctime(&server_started));
    fprintf(fp,"Total requests: %d\n",server_requests);
    fprintf(fp,"Bytes sent out: %d\n",server_bytes_sent);
    fclose(fp);
    return true;
}

int http_reply(int fd, FILE **fpp, int code, char *msg, char *type, char* content)
{
    FILE* fp=fdopen(fd,"w");
    int bytes=0;
    if(fp!=NULL){
        bytes+=fprintf(fp,"HTTP/ 1.0 %d %s\r\n",code,content);//why "HTTP/1.0" cannot send?
        bytes+=fprintf(fp,"Content-type:%s\r\n\r\n",type);
        if(content){
            bytes+=fprintf(fp,"%s\r\n",content);
        }
    }
    fflush(fp);
    if(fpp){
        *fpp=fp;
    }else{
        fclose(fp);
    }
    return bytes;
}


void not_implemented(int fd){
    http_reply(fd,NULL,501,"Not Implemented","text/plain","That command is not implemented");

}

void do_404(char *arg, int fd){
    http_reply(fd,NULL,404,"Not Found","text/plain","The item you seek is not here");
}

bool isadir(char *arg){
    struct stat info;
    return (stat(arg,&info)!=-1&&S_ISDIR(info.st_mode));
}

bool not_exist(char *arg, int fd)
{
    struct stat info;
    return stat(arg,&info)==-1;
}

void do_ls(char *arg, int fd){
    DIR *dirptr;
    struct dirent* direntp;
    FILE* fp;
    int bytes=0;
    bytes+=http_reply(fd,&fp,200,"OK","text/plain",NULL);
    bytes+=fprintf(fp,"Listing of Directory %s\n",arg);

    if((dirptr=opendir(arg))!=NULL){
        while(direntp=readdir(dirptr)){
            bytes+=fprintf(fp,"%s\n",direntp->d_name);
        }
        closedir(dirptr);
    }
    fclose(fp);
    server_bytes_sent+=bytes;
}

char *file_type(char* arg){
    char* cp;
    if((cp=strrchr(arg,'.'))!=NULL){
        return cp+1;
    }
    return " -";
}

void do_cat(char *arg, int fd)
{
    char* extension=file_type(arg);
    char* type="text/plain";
    FILE* fpsock,*fpfile;
    int c;
    int bytes;
    if(strcmp(extension,"html")==0){
        type="text/html";
    }else if(strcmp(extension,"gif")==0){
        type="text/gif";
    }else if(strcmp(extension,"jpg")==0){
        type="text/jpg";
    }else if(strcmp(extension,"jpeg")==0){
        type="text/jpeg";
    }

    fpsock=fdopen(fd,"w");
    fpfile=fopen(arg,"r");
    if(fpsock!=NULL&&fpfile!=NULL){
        bytes+=http_reply(fd,&fpsock,200,"OK",type,NULL);
        while((c=getc(fpfile))!=EOF){
            putc(c,fpsock);
            bytes++;
        }

        fclose(fpfile);
        fclose(fpsock);
    }
    server_bytes_sent+=bytes;
}


