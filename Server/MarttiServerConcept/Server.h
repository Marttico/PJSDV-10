
#ifndef SERVER_H_
#define SERVER_H_
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <errno.h> 

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

class Server {
public:
    Server(int);
    ~Server();
    void checkClient();
    int readMessage(char*);
    int sendMessage(char*);
    bool isConnected() const;
private:
    void resetClient();
    int port,server_fd, new_socket, opt, addrlen, noDelay, connected;
    struct sockaddr_in address;
    struct timeval messageTimeout,listenTimeout;  
};

#endif
