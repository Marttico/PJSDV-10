#include "Server.h"
Server::Server(int port):port(port)
{
    //create a master socket
    //timeSendRead.tv_usec = waitTimeUSec;
    //timeSendRead.tv_sec = 0;

    int addrlen = sizeof(address);
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections ,
    //this is just a good habit, it will work without this

    if( setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, (char *)&opt, sizeof(opt)) )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    cout <<"line: " <<__LINE__<<endl;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port );

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    cout<<"LISTENING ON PORT: "<< port<<endl;
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    addrlen = sizeof(address);
    cout <<"Waiting for connections ..."<<endl;
}

bool Server::setUp()
{
    //int flags = fcntl(new_socket, F_GETFL, 0);
    //fcntl(new_socket, F_SETFL, flags| O_NONBLOCK);
    cout <<"line: " <<__LINE__<<endl;

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    cout  <<"SOCKET"<<new_socket<<endl;
    //nonblocking

    //inform user of socket number - used in send and receive commands
    printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
    //send new connection greeting message


    if( send(new_socket, message, strlen(message), 0) != strlen(message) )
    {
        perror("send");
    }
    cout<<"Welcome message sent successfully"<<endl;
    return true;

}





uint64_t Server::readServer()
{
    uint64_t hasClients = 0;
    //if (setUp(0, 6000))
    //{

    //new_socket = new_socket;

    //cout <<"ReadyToSend!"<<endl;

    //if (FD_ISSET( sd, &readfds))
    //{
    int flags = fcntl(new_socket, F_GETFL, 0);
    if(fcntl(new_socket, F_SETFL, flags| O_NONBLOCK) == -1)
    {
        cout <<"erro  r"<<endl;
        return 0;
    }
    char buffer[1025] = {'\0'};
    //cout <<"line: " <<__LINE__<<endl;

    if ((valread = read( new_socket, buffer, 1024)) == 0)
    {
        //Somebody disconnected , get his details and print
        getpeername(new_socket, (struct sockaddr*)&address, \
                    (socklen_t*)&addrlen);
        printf("Host disconnected , ip %s , port %d \n",
               inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        //Close the socket and mark as 0 in list for reuse
        close( new_socket );

        new_socket = 0;
        return 0;
    }
    else if (valread == -1)
    {
        //cout<<"empty<"<<endl;
        return 0;
    }
    else
    {
        //printf("ID %i: Byte1: %i, Byte2: %i, Byte3: %i, Byte4: %i, Byte5: %i\n\n",i,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
        printf("ID %i: Byte1: %i, Byte2: %i, Byte3: %i, Byte4: %i, Byte5: %i\n\n",new_socket,buffer[0],((buffer[1]&0x03) << 8) + buffer[2],buffer[3],buffer[4]);
        cout <<":::" <<buffer<<endl;
/// TODO (mike#9#): Server.cpp:154:106: warning: left shift count >= width of type [-Wshift-count-overflow] ...
///  154 | rn buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24) + (buffer[4] << 32);
///      |                                                                            ~~~~~~~~~~^~~~~
///
        return buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24) + (buffer[4] << 32);

        //printf("%s",buffer);
    }
    // }

}

int Server::writeServer(char msg)
{
    //cout <<"line: " <<__LINE__<<endl;
    char MessageBuffer[2] = {msg,'\0'};
    char temp[5];

    int flags = fcntl(new_socket, F_GETFL, 0);
    fcntl(new_socket, F_SETFL, flags| O_NONBLOCK);
    //cout <<"line: " <<__LINE__<<endl;
    //printf("ID %i: Byte1: %i, Byte2: %i, Byte3: %i, Byte4: %i, Byte5: %i\n\n",i,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
    int valSend = send(new_socket, MessageBuffer, strlen(MessageBuffer),0);
    if(valSend == 0)
    {
        cout<<"0"<<endl;
    }
    else if (valSend==-1)
    {
        cout<<"-1"<<endl;

    }
    else
    {
        //cout<<"el";
    }

    //}


}