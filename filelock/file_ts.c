/*file lock demo
server*/
#include<stdio.h>
#include<fcntl.h>
#include<time.h>
#include<sys/file.h>
#include<unistd.h>
#include<string.h>
#define oops(m,x){perror(m);exit(x);}


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
    int fd;
    time_t now;
    char* message;
    if(argc!=2){
        fprintf(stderr,"usage:./file_ts filename\n");
        exit(1);
    }
    fd=open(argv[1],O_CREAT|O_TRUNC|O_WRONLY,0644);
    if(fd==-1) oops("open",2);
    while(1)
    {
        time(&now);
        message=ctime(&now);
        lock_operation(fd,F_WRLCK);
        lseek(fd,0,SEEK_SET);
        write(fd,message,strlen(message));
        lock_operation(fd,F_UNLCK);
        sleep(1);
    }
}
