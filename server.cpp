#include "server.h"

Server::Server() {
    messageSize = 512;
    port = 1115;
    ringBuffer.set_capacity(100);
    producing = true;
}


int Server::create_server_socket (){
    int l;
    int sfd; //Socket ID

    sfd = socket(AF_INET,SOCK_DGRAM,0); //Domain, type and protocol
    if (sfd == -1) {
        cout<<"Server socket failed \n";
        system("pause");
    }

    //Server address structure preparation
    l=sizeof(struct sockaddr_in);
    bzero(&sock_serv,l); //Write l bytes from &sock_serv with 0 = Inicializa el struct

    sock_serv.sin_family=AF_INET; //IP
    sock_serv.sin_port=htons(port); //Port given
    sock_serv.sin_addr.s_addr=htonl(INADDR_ANY); //IP no especificada

    //Connect the server socket
    cout<<"Waiting for connection on port "<<port<<"\n";
    if(bind(sfd,(struct sockaddr*)&sock_serv,l)==-1){ //Socket ID, structure and length
        cout<<"Server bind failed \n";
    }

    return sfd; //Return socket ID
}


void Server::server_producer_thread() {
    long packetsCounter=0, nBytesRead=0;
    char buf[messageSize];
    unsigned int l=sizeof(struct sockaddr_in); //Length
    int sfd = create_server_socket();

    cout<<"Producer thread waiting for data input. \n";

    do {
        nBytesRead = recvfrom(sfd,&buf,messageSize,0,(struct sockaddr *)&clt,&l);
        if (nBytesRead != 0) {
            string auxString(buf, messageSize);
            if (ringBuffer.full()==true) {
                cout<<"Buffer was full, old package was overwritten \n";
            }
            ringBuffer.push_back(auxString);
            packetsCounter++;
        }
    } while (nBytesRead != 0);

    cout<<packetsCounter<<" packets were introduced in the buffer. \n";
    producing = false;
}


void Server::server_consumer_thread() {
    int fd;
    char filename[256];

    //Date variable
    time_t intps;
    struct tm* tmi;

    intps = time(NULL);
    tmi = localtime(&intps);
    bzero(filename,256);
    sprintf(filename,"clt.%d.%d.%d.%d.%d.%d",tmi->tm_mday,
            tmi->tm_mon+1,1900+tmi->tm_year,tmi->tm_hour,tmi->tm_min,tmi->tm_sec);
    cout<<"Creating the output file : "<<filename<<"\n";

    //Open the file
    //It creates, or truncates the file to write on it.
    //0600 = chmod 600 = owner can read and write
    if((fd=open(filename,O_CREAT|O_WRONLY|O_TRUNC,0600))==-1){
        cout<<"File open failed \n";
        system("pause");
    }

    while (producing == true || ringBuffer.empty() == false) {
        if (ringBuffer.empty() == false) {
            const string &auxString2 = ringBuffer.front();
            //File descriptor, content, nbytes to write
            if ((write(fd,auxString2.c_str(), auxString2.size())) == -1) {
                cout<<"Error writing file. \n";
                system("pause");
            }
            ringBuffer.pop_front();
        }
    }
}

void Server::server_routine() {

}


void Server::old_server_routine() {
        int fd, sfd; //IDs

        char buf[messageSize];
        long count=0, n;
        char filename[256];
        unsigned int l=sizeof(struct sockaddr_in); //Length

        //Date variable
        time_t intps;
        struct tm* tmi;

        sfd = create_server_socket();

        intps = time(NULL);
        tmi = localtime(&intps);
        bzero(filename,256);
        sprintf(filename,"clt.%d.%d.%d.%d.%d.%d",tmi->tm_mday,
                tmi->tm_mon+1,1900+tmi->tm_year,tmi->tm_hour,tmi->tm_min,tmi->tm_sec);
        cout<<"Creating the output file : "<<filename<<"\n";

        //Open the file
        //It creates, or truncates the file to write on it.
        //0600 = chmod 600 = owner can read and write
        if((fd=open(filename,O_CREAT|O_WRONLY|O_TRUNC,0600))==-1){
            cout<<"File open failed \n";
            system("pause");
        }

        //Communication
        bzero(&buf,messageSize); //Initialize
        //Socket ID, buffer, lengths to be transfered, flags, sender struct
        //sender struct length.
        //Sender struct = If you do not care to identify the sender, you can
        //set both of these to zero but you will then have no way to
        //reply to the sender.
        cout<<"Waiting for data input. \n";
        n=recvfrom(sfd,&buf,messageSize,0,(struct sockaddr *)&clt,&l);
        string auxString(buf, messageSize);
        ringBuffer.push_back(auxString);

        while(n){
            //printf("%lld of data received \n",n);
            //cout<<n<<" of data received \n";

            count++;
            const string &auxString2 = ringBuffer.front();
            if ((write(fd,auxString2.c_str(),n)) == -1) { //File descriptor, content, nbytes to write
                cout<<"Error writing file. \n";
                system("pause");
            }
            ringBuffer.pop_front();
            bzero(buf,messageSize);
            n=recvfrom(sfd,&buf,messageSize,0,(struct sockaddr *)&clt,&l);
            string auxString3(buf, messageSize);
            ringBuffer.push_back(auxString3);
        }

        //printf("Nombre d'octets transférés : %lld \n",count);
        cout<<count<<" packets received by the server. \n";

        close(sfd);
        close(fd);
}
