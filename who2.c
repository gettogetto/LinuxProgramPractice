#include<stdio.h>
#include<utmp.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>
#define SHOWHOST
void show_info(struct utmp* utbufp);
int main(){
    struct utmp current_record;
    int utmpfd;
    int reclen=sizeof(current_record);

    if((utmpfd=open(UTMP_FILE,O_RDONLY))==-1)
    {
        perror(UTMP_FILE);//in stdio.h
        exit(1);
    }
    while(read(utmpfd,&current_record,reclen)==reclen){
        if(current_record.ut_type==7) //only show the login user
            show_info(&current_record);
    }
    close(utmpfd);
    return 0;
}

void show_info(struct utmp *utbufp){
    printf("% -8.8s",utbufp->ut_user);
    printf(" ");
    printf("% -8.8s",utbufp->ut_line);
    printf(" ");
    //printf("% 10ld",utbufp->ut_tv);
    printf("%12.12s",ctime(&utbufp->ut_tv));
    printf(" ");
#ifdef SHOWHOST
    if(utbufp->ut_host[0]!='\0'){
        printf("(%s)",utbufp->ut_host);
    }
#endif
    printf("\n");
}
