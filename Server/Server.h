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
#include <fcntl.h>
using namespace std;
class Server
{
    public:
        Server(int);
        bool setUp();
        int writeServer(char);
        uint64_t readServer();
        int disconnectHost();
    private:
        //int master_socket, addrlen, new_socket, activity, i, valread, sd, port;
        int server_fd, new_socket, valread;
        int port, addrlen;
        int valSend;
        //struct timeval timeAct;
        //struct timeval timeSendRead;
        int opt = true;
        //int max_sd;
        struct sockaddr_in address;
        char buffer[1025];
        //fd_set readfds;
        //int LedIntensity = 0;
        char *message = "ECHO Daemon v1.0 \r\n";


};