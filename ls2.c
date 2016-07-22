#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
/*****************************/
//printf file properties

#include<sys/stat.h>//for stat
#include<sys/types.h>
#include<time.h>//for ctime
#include<string.h>
#include<pwd.h>//for passwd
#include<grp.h>
void mode_to_letters(int mode,char str[]){
    strcpy(str,"----------");
    if(S_ISDIR(mode)) str[0]='d';
    if(S_ISCHR(mode)) str[0]='c';
    if(S_ISBLK(mode)) str[0]='b';

    if(mode&S_IRUSR) str[3]='r';
    if(mode&S_IWUSR) str[4]='w';
    if(mode&S_IXUSR) str[5]='x';

    if(mode&S_IRGRP) str[6]='r';
    if(mode&S_IWGRP) str[7]='w';
    if(mode&S_IXGRP) str[8]='x';

    if(mode&S_IROTH) str[9]='r';
    if(mode&S_IWOTH) str[10]='r';
    if(mode&S_IXOTH) str[11]='x';
}

char* uid_to_name(uid_t id){
    struct passwd* tmp=getpwuid(id);
    if(tmp!=NULL) return getpwuid(id)->pw_name;
    else{
        static char numstr[10];
        sprintf(numstr,"%d",id);
        return numstr;
    }
}
char* gid_to_name(gid_t id){

    struct group* tmp=getgrgid(id);
    if(tmp!=NULL) return getgrgid(id)->gr_name;
    else{
        static char numstr[10];
        sprintf(numstr,"%d",id);
        return numstr;
    }
}

void show_stat_info(char* fname,struct stat* buf){
    char mode_str[10];
    mode_to_letters(buf->st_mode,mode_str);
    printf("%s",mode_str);
    printf("%4d  ",buf->st_nlink);
    printf("%-8s ",uid_to_name(buf->st_uid));
    printf("%-8s ",gid_to_name(buf->st_gid));
    printf("%8ld ",buf->st_size);
    printf("%.12s ",ctime(&buf->st_mtim));
    printf("%s\n",fname);

}




/*****************************/
void do_ls(char*);
void do_stat(char*);
int main(int argc,char* argv[])
{
    if(argc==1){
        do_ls(".");
    }else{
        int i;
        for(i=1;i<argc;i++){
            do_ls(argv[i]);
        }
    }
}
void do_stat(char* filename){
    struct stat infobuf;
    if(stat(filename,&infobuf)==0){
        show_stat_info(filename,&infobuf);
    }else{
        perror(filename);
    }
}

void do_ls(char* dirname)
{
    DIR* dir_ptr;
    struct dirent* direntp;
    if((dir_ptr=opendir(dirname))==NULL){
        fprintf(stderr,"ls1:can not open %s\n",dirname);
    }else{
        while((direntp=readdir(dir_ptr))!=NULL){
            do_stat(direntp->d_name);
        }
        close(dir_ptr);
    }
}
