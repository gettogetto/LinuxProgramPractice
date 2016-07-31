#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

#define YES 1
#define NO 0

char* next_cmd(char* prompt,FILE* fp);
char** splitline(char*);
void freelist(char**);
void *emalloc(size_t);
void* erealloc(void*,size_t);
int execute(char**);
void fatal(char*,char*,int);
