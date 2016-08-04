#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#define oops(m,x) {perror(x);exit(x);}

void fatal(char* message){
    fprintf(stderr,message);
    exit(1);
}

void be_dc(int in[2],int out[2])
{
    if(dup2(in[0],0)==-1) oops("dc",3);
    close(in[0]);
    close(in[1]);

    if(dup2(out[1],1)==-1) oops("dc",4);
    close(out[1]);
    close(out[0]);

    execlp("dc","dc","-",NULL);
    oops("can not run",5);

}


void be_bc(int todc[2],int fromdc[2]){
    int num1,num2;
    char operation[BUFSIZ],message[BUFSIZ];

    FILE* fpout,*fpin;

    close(todc[0]);
    close(fromdc[1]);

    fpout=fdopen(todc[1],"w");
    fpin=fdopen(fromdc[0],"r");

    while(printf("tinybc: "),fgets(message,BUFSIZ,stdin)!=NULL){
        if(sscanf(message,"%d%[+ - * / ^]%d",&num1,operation,&num2)!=3){
            printf("syntax error\n");
            continue;
        }
        if(fprintf(fpout,"%d\n%d\n%c\np\n",num1,num2,*operation)==EOF){
            fatal("error writing");
        }
        fflush(fpout);//important!!
        if(fgets(message,BUFSIZ,fpin)==NULL) break;
        printf("%d %c %d = %s",num1,*operation,num2,message);
    }
    fclose(fpout);
    fclose(fpin);
}

int main(){
    int pid,todc[2],fromdc[2];
    //make 2 pipes

    if(pipe(todc)==-1||pipe(fromdc)==-1){
        oops("pipe failed",1);
    }
    if((pid=fork())==-1){
        oops("fork failed",1);
    }
    if(pid==0){
        be_dc(todc,fromdc);
    }else{
        be_bc(todc,fromdc);
        wait(NULL);
    }
}
