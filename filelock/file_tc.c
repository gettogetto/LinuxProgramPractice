#include<sys/file.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#define oops(m,x){perror(m);exit(x);}
#define BUFLEN 10

void lock_operation(int fd,int op)
{
    struct flock lock;
    lock.l_whence=SEEK_SET;//start of the file
    lock.l_start=lock.l_len=0;
    lock.l_pid=getpid();
    lock.l_type=op;
    if(fcntl(fd,F_SETLK,&lock)==-1) oops("lock operation",1);
    return;
}
int main(int argc,char* argv[])
{
    int fd,nread;
    char buf[BUFLEN];
    if(argc!=2){
        fprintf(stderr,"usage:./file_tc filename\n");
        exit(1);
    }
    fd=open(argv[1],O_RDONLY);
    if(fd==-1) oops(argv[1],O_RDONLY);
    lock_operation(fd,F_RDLCK);
    while((nread=read(fd,buf,BUFLEN))>0){
        write(STDOUT_FILENO,buf,nread);
    }
    lock_operation(fd,F_UNLCK);
    close(fd);
}

