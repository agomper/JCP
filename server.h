#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>

// Time function, sockets, htons... file stat
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

// File function and bzero
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

#include <iostream>
#include <unistd.h>
using namespace std;

class Server {
    int bufferSize;
    struct sockaddr_in sock_serv; //Server address structure
    struct sockaddr_in clt; //Client address structure
    int port;
public:
    Server();
    int create_server_socket ();
    void server_routine();
};

#endif // SERVER_H
