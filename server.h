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
#include "circularbuffer.h"
#include <boost/circular_buffer.hpp>
#include <pthread.h>
using namespace std;

class Server {
    int messageSize, port;
    bool producing;
    struct sockaddr_in sock_serv; //Server address structure
    struct sockaddr_in clt; //Client address structure
    boost::circular_buffer<string> ringBuffer; //CircularBuffer
public:
    Server();
    int create_server_socket ();
    void old_server_routine();
    void server_routine();
    void server_producer_thread();
    void server_consumer_thread();
};

#endif // SERVER_H
