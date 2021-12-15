#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

using namespace std;
class Server
{
    public:
        Server(int, int);
        void loop();
        int writeServer(uint8_t);
        uint64_t readServer();
    private:
        int master_socket, addrlen, new_socket , max_clients, activity, i, valread , sd, port;
        struct timeval tv;
        vector<int> client_socket;
        int opt = true;
        int max_sd; 
        struct sockaddr_in address;
        char buffer[1025];
        fd_set readfds;
        int LedIntensity = 0;
        char *message = "ECHO Daemon v1.0 \r\n";


};