#ifndef DGRAM_H
#define DGRAM_H
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>



int make_internet_address(char* hostname,int portnum,struct sockaddr_in* addrp);
int make_dgram_server_socket(int portnum);
int make_internet_address(char* hostname,int portnum,struct sockaddr_in* addrp);
int get_internet_address(char* host,int len,int *portp,struct sockaddr_in* addrp);

int make_dgram_client_socket();
#endif
