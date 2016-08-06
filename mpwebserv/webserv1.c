/* multi process webserv
 * usage:ws portnumber
 * GET support only
 * method:fork a new child to handle each request
 * many weakness only demo
 *
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdbool.h>
#include"socklib.h"
#define oops(msg,num){perror(msg);exit(num);}


void read_til_crnl(FILE* fpin);
void process_rq(char* request,int fd);


int main(int argc,char* argv[])
{
    int sock,fd;
    FILE* fpin;
    char request[BUFSIZ];
    if(argc!=2){
        fprintf(stderr,"usage:./webserv1 portnum\n");
        exit(1);
    }
    sock=make_server_socket(atoi(argv[1]));
    if(sock==-1) oops("make_server_socket",2);

    /*main loop*/
    while(1){
        fd=accept(sock,NULL,NULL);
        if(fd==-1) oops("accept",3);
        fpin=fdopen(fd,"r");
        if(fpin==NULL) oops("fdopen",4);
        fgets(request,BUFSIZ,fpin);
        printf("got a call:request = %s\n",request);

        /*skip over arguments*/
        read_til_crnl(fpin);
        /*do what client asks*/

        process_rq(request,fd);
        fclose(fpin);
    }

}
void header(FILE* fp,char* content_type){
    fprintf(fp,"HTTP 1.0 200 OK\r\n");
    if(content_type){
        fprintf(fp,"Content=type:%s\r\n",content_type);
    }
}

void cannot_do(int fd){
    FILE* fp=fdopen(fd,"w");
    fprintf(fp,"HTTP 1.0 501 NOt Implemented\r\n");
    fprintf(fp,"Content-type:text plain\r\n");
    fprintf(fp,"\r\n");

    fprintf(fp,"That command is not yet implemented\r\n");
    fclose(fp);
}

void do_404(char* item,int fd)
{
    FILE* fp=fdopen(fd,"w");
    fprintf(fp,"HTTP 1.0 404 not found\r\n");
    fprintf(fp,"Content-type:text plain\r\n");
    fprintf(fp,"\r\n");
    fprintf(fp,"The item you requested:%s\r\nis not found\r\n",item);
    fclose(fp);
}

bool isadir(char* f){
    struct stat info;
    return (stat(f,&info)!=-1&&S_ISDIR(info.st_mode));
}

bool not_exist(char* f){
    struct stat info;
    return stat(f,&info)==-1;

}


void do_ls(char* dir,int fd)
{
    FILE*  fp;
    fp=fdopen(fd,"w");
    header(fp,"text/plain");
    fprintf(fp,"\r\n");
    fflush(fp);

    dup2(fd,1);
    dup2(fd,2);
    close(fd);

    execlp("ls","ls","-l",dir,NULL);
    perror(dir);
    exit(1);
}

char* file_type(char* f){
    char* cp;
    if((cp=strrchr(f,'.'))!=NULL){//strrchr find  '.' from right,strchr from left
        return cp+1;
    }
    return "";
}

bool ends_in_cgi(char* f){
    return strcmp(file_type(f),"cgi")==0;
}

void do_exec(char* prog,int fd)
{
    FILE* fp;
    fp=fdopen(fd,"w");
    header(fp,NULL);

    fflush(fp);
    dup2(fd,1);
    dup2(fd,2);
    close(fd);
    execl(prog,prog,NULL);
    perror(prog);
}

void do_cat(char* f,int fd)
{
    char* extension=file_type(f);
    char* content="text/plain";
    FILE* fpsock,*fpfile;
    int c;
    if(strcmp(extension,"html")==0){
        content="text/html";
    }else if(strcmp(extension,"gif")==0){
        content="image/gif";
    }else if(strcmp(extension,"jpeg")==0){
        content="image/jpeg";
    }

    fpsock=fdopen(fd,"w");
    fpfile=fopen(f,"r");
    if(fpsock!=NULL&&fpfile!=NULL){
        header(fpsock,content);
        fprintf(fpsock,"\r\n");
        while((c=getc(fpfile))!=EOF){//read from the file and send the content
            putc(c,fpsock);
        }
        fclose(fpfile);
        fclose(fpsock);

    }
    exit(0);
}

void read_til_crnl(FILE *fpin)
{
    char buf[BUFSIZ];
    while(fgets(buf,BUFSIZ,fpin)!=NULL&&strcmp(buf,"\r\n")!=0);
}

void process_rq(char *request, int fd)
{
    char cmd[BUFSIZ],arg[BUFSIZ];
    /*create a child ,if not child return */
    if(fork()!=0) return ;
    strcpy(arg,"./");
    if(sscanf(request,"%s %s",cmd,arg)!=2) return;

    if(strcmp(cmd,"GET")!=0)
        cannot_do(fd);
    else if(not_exist(arg))
        do_404(arg,fd);
    else if(isadir(arg))
        do_ls(arg,fd);
    else if(ends_in_cgi(arg))
        do_exec(arg,fd);
    else
        do_cat(arg,fd);
}


