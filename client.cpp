#include "client.h"

Client::Client() {
    bufferSize = 1500;
    port = 1115;
}

//This function let us know the duration of the file transfer
int Client::duration (struct timeval *start,struct timeval *stop,struct timeval *delta)
{
    suseconds_t microstart, microstop, microdelta;

    microstart = (suseconds_t) (100000*(start->tv_sec))+ start->tv_usec;
    microstop = (suseconds_t) (100000*(stop->tv_sec))+ stop->tv_usec;
    microdelta = microstop - microstart;

    delta->tv_usec = microdelta%100000;
    delta->tv_sec = (time_t)(microdelta/100000);

    if((*delta).tv_sec < 0 || (*delta).tv_usec < 0)
        return -1;
    else
        return 0;
}


//Client socket creation
int Client::create_client_socket (){
    int l;
    int sfd;

    sfd = socket(AF_INET,SOCK_DGRAM,0);
    if (sfd == -1) {
        cout<<"Client socket failed \n";
        system("pause");
    }


    //Initializate
    l=sizeof(struct sockaddr_in);
    bzero(&sock_serv,l);

    sock_serv.sin_family=AF_INET;
    sock_serv.sin_port=htons(port);
    //Si no ha hecho la conversion IPv4 a IPv6 bien, falla
    if (inet_pton(AF_INET,ipAddress,&sock_serv.sin_addr)==0){
        cout<<"Invalid IP adress\n";
        return EXIT_FAILURE;
    }

    return sfd;
}


void Client::client_routine() {
    struct timeval start, stop, delta; //For duration
    int sfd,fd; //Socket and file ID
    char buf[bufferSize];
    long count=0, m, fileSize;
    long int n;
    int l=sizeof(struct sockaddr_in);
    struct stat fileInfo;


    //Client socket creation
    sfd=create_client_socket();
    if (sfd < 0) {
        cout<<"Error creation client socket. \n";
        system("pause");
        //return EXIT_FAILURE
    }

    cout<<"Enter file name: \n";
    cin>>filename;

    if ((fd = open(filename,O_RDONLY))==-1){
        cout<<"File open failed. \n";
        system("pause");
        //return EXIT_FAILURE;
    }

    //File descriptor of the file, struct where data about
    //the file will be stored.
    if (stat(filename,&fileInfo)==-1){
        cout<<"Stat failed. \n";
        //return EXIT_FAILURE;
    }
    else {
        fileSize=fileInfo.st_size;
    }


    //Initializate
    bzero(&buf,bufferSize);

    gettimeofday(&start,NULL);
    //File descriptor to read, Buffer where the content will
    //be srote, number of bytes to read. Returns numbers of
    //bytes read
    n=read(fd,buf,bufferSize);
    while(n) {
        if(n==-1){
            cout<<"Reading file failed. \n";
            //return EXIT_FAILURE;
        }
        //Socket, buffer, bytes to send, flag, structure and structure lenght
        m=sendto(sfd,buf,n,0,(struct sockaddr*)&sock_serv,l);
        if(m==-1){
            cout<<"Sending file failed. \n";
            //return EXIT_FAILURE;
        }
        count++;

        bzero(buf,bufferSize);
        //Entiendo que la funcion read() se encarga de mover
        //el puntero interno del fichero para saber por donde
        //seguir leyendo.
        n=read(fd,buf,bufferSize);
    }
    //Reads until it returns 0, end of file. EOF.

    //To release the connection with server.
    m=sendto(sfd,buf,0,0,(struct sockaddr*)&sock_serv,l);
    gettimeofday(&stop,NULL);
    duration(&start,&stop,&delta);

    printf("Number of packets sent: %lld\n",count);
    printf("File had a size of: %lld \n",fileSize);
    printf("The process took: %ld.%d \n",delta.tv_sec,delta.tv_usec);

    close(sfd);
    close(fd);
    //return EXIT_SUCCESS;
}
