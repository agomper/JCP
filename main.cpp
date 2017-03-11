#include <iostream>
#include <unistd.h>
#include "server.h"
#include "client.h"
using namespace std;

int main() {
    int option;
    Server serObj;
    Client cliObj;

    cout<<"Elige: 1.Servidor 2.Cliente \n";
    cin>>option;
    if (option==1) {
        serObj.server_routine();
    }
    else {
        cliObj.client_routine();
    }
}
