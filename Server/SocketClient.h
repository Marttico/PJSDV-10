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
class SocketClient
{
    public:
        SocketClient(string, int);
        ~SocketClient();
        int setup();
        void loop();
        void writeSocketClient(uint8_t);
        const uint8_t *readSocketClient();
    private:
        string ip;
        int port;
        int sock = 0, valread;
	    struct sockaddr_in serv_addr;

};
