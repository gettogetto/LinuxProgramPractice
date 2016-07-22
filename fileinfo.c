//printf file properties

#include<stdio.h>
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
    return getpwuid(id)->pw_name;
}
char* gid_to_name(gid_t id){
    return getgrgid(id)->gr_name;
}

void show_stat_info(char* fname,struct stat* buf){
    char mode_str[10];
    mode_to_letters(buf->st_mode,mode_str);
    printf("mode:%s\n",mode_str);
    printf("links:%d\n",buf->st_nlink);
    printf("user:%s\n",uid_to_name(buf->st_uid));
    printf("group:%s\n",gid_to_name(buf->st_gid));
    printf("size:%d\n",buf->st_size);
    printf("modtime:%s\n",ctime(&buf->st_mtim));
    printf("name:%s",fname);
}




