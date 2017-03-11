#include "server.h"

Server::Server() {
    bufferSize = 1500;
    port = 1115;
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

void Server::server_routine() {
        int fd, sfd; //IDs

        char buf[bufferSize];
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
        bzero(&buf,bufferSize); //Initialize
        //Socket ID, buffer, lengths to be transfered, flags, sender struct
        //sender struct length.
        //Sender struct = If you do not care to identify the sender, you can
        //set both of these to zero but you will then have no way to
        //reply to the sender.
        cout<<"Waiting for data input. \n";
        n=recvfrom(sfd,&buf,bufferSize,0,(struct sockaddr *)&clt,&l);

        while(n){
            //printf("%lld of data received \n",n);
            cout<<n<<" of data received \n";

            count++;
            write(fd,buf,n); //File descriptor, content, nbytes to write
            bzero(buf,bufferSize);
            n=recvfrom(sfd,&buf,bufferSize,0,(struct sockaddr *)&clt,&l);
        }

        //printf("Nombre d'octets transférés : %lld \n",count);
        cout<<count<<" packets received by the server. \n";

        close(sfd);
        close(fd);
}
