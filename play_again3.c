#include<stdio.h>
#include<fcntl.h>
#include<termios.h>
#include<string.h>
#include<stdlib.h>

#define ASK "Do you want another transaction"
#define TRIES 3
#define SLEEPTIME 2
#define BEEP putchar('\a')

int get_response(char* question,int maxtries){
    int input;
    printf("\n%s (y/n)?\n",question);
    fflush(stdout);
    while(1)
    {
        sleep(SLEEPTIME);
        input=tolower(get_ok_char());
        if(input=='y') return 0;
        if(input=='n') return 1;
        if(maxtries--==0) return 2;
        BEEP;
    }
}

int get_ok_char(){
    int c;
    while((c=getchar())!=EOF&&strchr("yYnN",c)==NULL){

    }
    return c;
}

void set_cr_noecho_mode(){
    struct termios ttystate;
    tcgetattr(0,&ttystate);
    ttystate.c_lflag&=~ICANON;
    ttystate.c_lflag&=~ECHO;//set no echo
    ttystate.c_cc[VMIN]=1;//set tty:get one char a time
    tcsetattr(0,TCSANOW,&ttystate);
}
void set_nodelay_mode(){
    int termflags;
    termflags=fcntl(0,F_GETFL);
    termflags|=O_NONBLOCK;//or O_NDELAY,set no blocking
    fcntl(0,F_SETFL,termflags);
}

void tty_mode(int how)
{
    static struct termios original_mode;
    static int origin_flag;
    if(how==0){
        tcgetattr(0,&original_mode);
        origin_flag=fcntl(0,F_GETFL);

    }else{
        tcsetattr(0,TCSANOW,&original_mode);
        fcntl(0,F_SETFL,origin_flag);
    }
}

int main(){
    tty_mode(0);
    set_cr_noecho_mode();
    set_nodelay_mode();

    int response;
    response=get_response(ASK,TRIES);

    tty_mode(1);
    return response;
}
