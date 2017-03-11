#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>

// Time function, sockets, htons... file stat
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

// File function and bzero
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#include <iostream>
#include <unistd.h>
using namespace std;

class Client {
    int bufferSize;
    int port;
    char ipAddress[10]="127.0.0.1";
    struct sockaddr_in sock_serv; //Structure address server
    char filename[50];
public:
    Client();
    int duration (struct timeval *start,struct timeval *stop, struct timeval *delta);
    int create_client_socket ();
    void client_routine();
};

#endif // CLIENT_H
