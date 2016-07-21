//improve who2.c using buff,read 16 utmp struct at one time.
//it can redude the system call'read'
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<utmp.h>
#include<time.h>
#define SHOWHOST
#define NRECS 16
#define NULLUT ((struct utmp*)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS*UTSIZE];
static int num_recs;
static int cur_rec;
static int fd_utmp=-1;

int utmp_open(char* filename){
    fd_utmp=open(filename,O_RDONLY);
    cur_rec=num_recs=0;
    return fd_utmp;
}
struct utmp* utmp_next(){
    struct utmp* recp;
    if(fd_utmp==-1) return NULLUT;
    if(cur_rec==num_recs&&utmp_reload()==0) return NULLUT;

    recp=(struct utmp*)&utmpbuf[cur_rec*UTSIZE];
    cur_rec++;
    return recp;
}
int utmp_reload(){
    int amt_read;
    amt_read=read(fd_utmp,utmpbuf,NRECS*UTSIZE);
    num_recs=amt_read/UTSIZE;//how many got?
    cur_rec=0;
    return num_recs;
}

void utmp_close(){
    if(fd_utmp!=-1){
        close(fd_utmp);
    }
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


int main(int argc,char* argv[]){
    struct utmp* utbufp;
    if(utmp_open(UTMP_FILE)==-1){
        perror(UTMP_FILE);
        exit(1);
    }
    while((utbufp=utmp_next())!=NULLUT){
        if(utbufp->ut_type==7){
            show_info(utbufp);
        }
    }
    utmp_close();

    return 0;
}
