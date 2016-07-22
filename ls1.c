#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>

void do_ls(char*);
int main(int argc,char* argv[])
{
    if(argc==1){
        do_ls(".");
    }else{
        for(int i=1;i<argc;i++){
            printf("%s: \n",argv[i]);
            do_ls(argv[i]);
        }
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
            printf("%s\n",direntp->d_name);
        }
        close(dir_ptr);
    }
}
