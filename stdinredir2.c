#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main(){
    int fd;
    int newfd;
    char line[100];
    fgets(line,100,stdin);printf("%s",line);
    fgets(line,100,stdin);printf("%s",line);
    fgets(line,100,stdin);printf("%s",line);

    fd=open("/etc/passwd",O_RDONLY);
    close(0);


    newfd=dup2(fd,0);//newfd=dup(fd);
    close(fd);

    fgets(line,100,stdin);printf("%s",line);
    fgets(line,100,stdin);printf("%s",line);
    fgets(line,100,stdin);printf("%s",line);
    exit(0);
}
