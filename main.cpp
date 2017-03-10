#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
using namespace std;

#define BUFSIZE 2048
#define PORT_NUMBER	21235

int main(int argc, char **argv) {
    struct sockaddr_in myaddr;	//Server address
    struct sockaddr_in remaddr; //Remote address
    socklen_t addrlen = sizeof(remaddr); //Length of the addresses
    int recvlen;			//Bytes received
    int fd;				//Our socket ID
    int msgcnt = 0;     //Counter # of messages we received
    char buf[BUFSIZE];	//Receiver buffer


    //Create a UDP socket
    //int s = socket(domain, type, protocol)
    //fd = Integer de valor pequeño si todo va bien
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        cout<<"Cannot create socket.\n";
        system("pause");
        return 0;
    }

    /* Bind the socket to any valid IP address and a specific port */

    memset((char *)&myaddr, 0, sizeof(myaddr)); //Para inicializar el struct a cero
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY); //CAMBIAR cuando uses 2 ordenadores. INTERFACE
    myaddr.sin_port = htons(PORT_NUMBER);

    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        cout<<"Bind failed \n";
        system("pause");
        return 0;
    }

    /*Loop, receiving data and printing what we received */
    while (1) {
        cout<<"Waiting on port"<<PORT_NUMBER<<" of IP "<<INADDR_ANY<<"\n";

        //recvfrom se queda esperando a que le llegue algo
        recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
        if (recvlen > 0) { //Ha recibido algo
            buf[recvlen] = 0;
            cout<<"Received message: "<<buf<<", of "<<recvlen<<" bytes \n";
        }
        else {
            cout<<"Something went wrong with recvfrom \n";
            system("pause");
        }

        cout<<buf<<" ack "<<msgcnt++<<"\n";
        cout<<"Sending response "<<buf<<"\n";
        if (sendto(fd, buf, strlen(buf), 0, (struct sockaddr *)&remaddr, addrlen) < 0) {
            cout<<"Error sending info to the client \n";
            system("pause");
        }
    }
    //close(fd); //It nevers exits, so it does not close the socket.
}
