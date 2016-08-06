#ifndef SOCKLIB_H
#define SOCKLIB_H
int make_server_socket_q(int portnum,int backlog);
int make_server_socket(int portnum);
int connect_to_server(char* host,int portnum);
#endif
