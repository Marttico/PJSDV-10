#include "Server.h"

Server::Server(int Port):port(Port),opt(1),noDelay(1),addrlen(sizeof(address)),connected(0){
    listenTimeout.tv_sec = 0;
    listenTimeout.tv_usec = 1000;
    messageTimeout.tv_sec = 0;
    messageTimeout.tv_usec = 500000;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt (server_fd, IPPROTO_TCP, TCP_NODELAY, &noDelay,
                    sizeof noDelay) < 0)
            perror("setsockopt failed\n");



    checkClient();
}

Server::~Server(){

}

void Server::checkClient(){
    connected = 0;
    //Set Listen Timeout
    if (setsockopt (server_fd, SOL_SOCKET, SO_RCVTIMEO, &listenTimeout,
                sizeof listenTimeout) < 0)
        perror("setsockopt failed\n");

    if (setsockopt (server_fd, SOL_SOCKET, SO_SNDTIMEO, &listenTimeout,
                sizeof listenTimeout) < 0)
        perror("setsockopt failed\n");
    // Listen for new sockets.
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    // Accept Socket
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
    {
        //perror("accept");
        //exit(EXIT_FAILURE);
    }else{
        connected = 1;
    }
    if(connected){
        //Set Message Timeout
        if (setsockopt (new_socket, SOL_SOCKET, SO_RCVTIMEO, &messageTimeout,
                    sizeof messageTimeout) < 0)
            perror("setsockopt failed\n");

        if (setsockopt (new_socket, SOL_SOCKET, SO_SNDTIMEO, &messageTimeout,
                    sizeof messageTimeout) < 0)
            perror("setsockopt failed\n");
        
        if (setsockopt (new_socket, IPPROTO_TCP, TCP_NODELAY, &noDelay,
                    sizeof noDelay) < 0)
            perror("setsockopt failed\n");
    }
}

int Server::readMessage(char* outstr){
    int valread = read(new_socket , outstr, 1024);
    if(valread == -1 && connected){
        resetClient();
    }else if(valread == -1 && !connected){
        resetClient();
        outstr[0] = 0;
    }
    return valread;
    
}

int Server::sendMessage(char* msg){
    return send(new_socket , msg , strlen(msg) , 0 );
}

void Server::resetClient(){
    if(connected){
        perror("Client Disconnected, Reconnect the device");
        close(new_socket);
        connected = 0;
    }
    checkClient();
}