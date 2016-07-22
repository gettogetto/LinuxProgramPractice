//printf size of passwd file

#include<stdio.h>
#include<sys/stat.h>
int main(int argc,char* argv[])
{
    struct stat infobuf;
    if(stat("/etc/passwd",&infobuf)==-1){//get file informaiton
        perror("/etc/passwd");
    }else{
        printf("The size of /etc/passwd is %d\n",infobuf.st_size);
    }
}
