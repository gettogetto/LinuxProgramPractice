#include<stdio.h>
#include<termios.h>
#define QUESTION "Do you want another transaction?"
int get_reponse(char* question)
{
    int input;
    printf("\n%s(y/n)\n",question);
    while(1)
    {
        switch(input=getchar())
        {
        case 'Y':
        case 'y':
            return 0;
        case 'N':
        case 'n':

        case EOF:
            return 1;
        default:
            printf("\ncan not understand %c\n",input);
            printf("type y/n \n");
        }
    }
}
//choose set_mode() or set_cr_noecho_mode()
void set_mode(){
    struct termios ttystate;
    tcgetattr(0,&ttystate);

    ttystate.c_lflag&=~ICANON;//set no buffer
    ttystate.c_cc[VMIN]=1;//set tty:get one char a time

    tcsetattr(0,TCSANOW,&ttystate);
}

void set_cr_noecho_mode(){
    struct termios ttystate;
    tcgetattr(0,&ttystate);
    ttystate.c_lflag&=~ICANON;
    ttystate.c_lflag&=~ECHO;//set no echo
    ttystate.c_cc[VMIN]=1;//set tty:get one char a time
    tcsetattr(0,TCSANOW,&ttystate);
}


void tty_mode(int how)
{
    static struct termios original_mode;
    if(how==0){
        tcgetattr(0,&original_mode);
    }else{
        return tcsetattr(0,TCSANOW,&original_mode);
    }
}

int main(){
    tty_mode(0);

    set_cr_noecho_mode();//set_mode();
    int response;
    response=get_reponse(QUESTION);

    tty_mode(1);
}
