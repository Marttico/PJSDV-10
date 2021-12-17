#include "Server.h"
Server::Server(int port, int waitTimeUSec):port(port), timeSendRead.tv_usec(waitTimeUSec)
{
    //create a master socket
    if((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections ,
    //this is just a good habit, it will work without this

    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    if(setsockopt(master_socket, SOL_SOCKET, SO_RCVTIMEO, &timeSendRead, sizeof(timeSendRead)) <0) //voor de tijd die hij krijgt om uit te lezen of te senden
    {
        perror("setsockopt2");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port );

    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    cout<<"LISTENING ON PORT: "<< port<<endl;
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    addrlen = sizeof(address);
    cout <<"Waiting for connections ..."<<endl;
}

bool Server::setUp(int aantalSec, int aantalUsec)
{
    //clear the socket set
    FD_ZERO(&readfds);
    FD_SET(master_socket, &readfds);
    max_sd = master_socket;

    if(max_sd > 0)
        FD_SET( max_sd, &readfds);

    //highest file descriptor number, need it for the select function


    timeAct.tv_sec = aantalSec;
    timeAct.tv_usec = aantalUsec;

    activity = select(max_sd+1, &readfds, NULL, NULL, &tv);
    /* Don't rely on the value of tv now! */

    if (activity < 0)
    {
        perror("select()");
        return false;
    }
    else if (activity)
    {

    }
    //printf("Data is available now.\n");
    /* FD_ISSET(0, &rfds) will be true. */
    else
    {
        //cout<<"No data within five seconds."<<endl;
        return false;
    }

    //If something happened on the master socket ,
    //then its an incoming connection
    if (FD_ISSET(master_socket, &readfds))
    {
        if ((new_socket = accept(master_socket,
                                 (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        //inform user of socket number - used in send and receive commands
        printf("New connection , socket fd is %d , ip is : %s , port : %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        //send new connection greeting message
        if( send(new_socket, message, strlen(message), 0) != strlen(message) )
        {
            perror("send");

        }

        cout<<"Welcome message sent successfully"<<endl;
        bool clientSocketWasEmpty = false;
        //add new socket to array of sockets
        for (i = 0; i < max_clients; i++)
        {
            //if position is empty
            if( client_socket[i] == 0 )
            {
                client_socket[i] = new_socket;
                clientSocketWasEmpty = true;

                //printf("Adding to list of sockets as %d\n" , i);

                break;
            }

        }
        if(!clientSocketWasEmpty)
        {
            cout <<"No empty sockets. Please do not connect to socket " <<endl;
        }
    }


    return true;
}

uint64_t Server::readServer()
{
    uint64_t hasClients = 0;
    if (setUp(0, 6000))
    {
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];
            if(sd == 0)
            {
                break;
            }
            else
            {
                hasClients =1;
            }
            //cout <<"ReadyToSend!"<<endl;

            if (FD_ISSET( sd, &readfds))
            {
                char buffer[1025] = {'\0'};
                if ((valread = read( sd, buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd, (struct sockaddr*)&address, \
                                (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n",
                           inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );

                    client_socket[i] = 0;
                    return 0;
                }
                else
                {
                    //printf("ID %i: Byte1: %i, Byte2: %i, Byte3: %i, Byte4: %i, Byte5: %i\n\n",i,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
                    printf("ID %i: Byte1: %i, Byte2: %i, Byte3: %i, Byte4: %i, Byte5: %i\n\n",i,buffer[0],((buffer[1]&0x03) << 8) + buffer[2],buffer[3],buffer[4]);
                    cout <<":::" <<buffer<<endl;
                    return buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24) + (buffer[4] << 32);
                    //printf("%s",buffer);
                }
            }

        }

    }
    else
    {
        return 0;
    }
    return hasClients;


}

int Server::writeServer(char msg)
{
    int hasClients = 0;
    for (i = 0; i < max_clients; i++)
    {
        sd = client_socket[i];
        if(sd)
        {
            //gelukkig leeft er ergens een client
            hasClients = 1;
        }
        /// Let op: eerst read, controleer of client nog bestaat, dan pas send. Send crashed als geen controle!!!


        cout <<"line: " <<__LINE__<<endl;
        char MessageBuffer[2] = {msg,'\0'};
        char temp[5];
        // if (FD_ISSET( sd, &readfds))
        //{
        //char temp[1024] = {'\0'};
        cout <<"line: " <<__LINE__<<endl;
        if ((valread = read( sd, temp, 1)) == 0)
        {
            //Somebody disconnected , get his details and print
            getpeername(sd, (struct sockaddr*)&address, \
                        (socklen_t*)&addrlen);
            printf("Host disconnected , ip %s , port %d \n",
                   inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            //Close the socket and mark as 0 in list for reuse
            close( sd );

            client_socket[i] = 0;
            return 0;
        }
        else
        {
            cout <<"line: " <<__LINE__<<endl;
            //printf("ID %i: Byte1: %i, Byte2: %i, Byte3: %i, Byte4: %i, Byte5: %i\n\n",i,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
            return send(sd, MessageBuffer, strlen(MessageBuffer),0);
            //}
            //printf("%s",buffer);
        }
        /* }
         else
         {
             cout <<"line: " <<__LINE__<<endl;
             //return send(sd, MessageBuffer, strlen(MessageBuffer),0);
         }*/



        //}
    }


}