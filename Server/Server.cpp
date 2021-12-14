#include "Server.h"
Server::Server(int maxClients, int port): max_clients(maxClients), port(port), client_socket(maxClients, 0)
{
    //create a master socket
    if((master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
    
    //set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0 )
	{
		perror("setsockopt");
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

void Server::loop()
{
    //clear the socket set
    FD_ZERO(&readfds);

    //add master socket to set
    FD_SET(master_socket, &readfds);
    max_sd = master_socket;
        
    //add child sockets to set
    for ( i = 0 ; i < max_clients ; i++)
    {
        //socket descriptor
        sd = client_socket[i];
            
        //if valid socket descriptor then add to read list
        if(sd > 0)
            FD_SET( sd , &readfds);
            
        //highest file descriptor number, need it for the select function
        if(sd > max_sd)
            max_sd = sd;
    }
    tv.tv_sec = 0;
    tv.tv_usec = 3000;

    activity = select(max_sd+1, &readfds, NULL, NULL, &tv);
    /* Don't rely on the value of tv now! */

    if (activity < 0)
        perror("select()");
    else if (activity){}
        //printf("Data is available now.\n");
        /* FD_ISSET(0, &rfds) will be true. */
    else
    {
        //cout<<"No data within five seconds."<<endl;
        return;
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
        printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
        //send new connection greeting message
        if( send(new_socket, message, strlen(message), 0) != strlen(message) )
        {
            perror("send");
            
        }
            
        cout<<"Welcome message sent successfully"<<endl;
            
        //add new socket to array of sockets
        for (i = 0; i < max_clients; i++)
        {
            //if position is empty
            if( client_socket[i] == 0 )
            {
                client_socket[i] = new_socket;

                //printf("Adding to list of sockets as %d\n" , i);
                    
                break;
            }
        }
    }
        
    //else its some IO operation on some other socket
    /*for (i = 0; i < max_clients; i++)
    {
        sd = client_socket[i];
            
        if (FD_ISSET( sd , &readfds))
        {
            //Check if it was for closing , and also read the
            //incoming message
            char buffer[1025] = {'\0'};
            if ((valread = read( sd , buffer, 1024)) == 0)
            {
                //Somebody disconnected , get his details and print
                getpeername(sd , (struct sockaddr*)&address , \
                    (socklen_t*)&addrlen);
                printf("Host disconnected , ip %s , port %d \n" ,
                    inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                    
                //Close the socket and mark as 0 in list for reuse
                close( sd );
                
                client_socket[i] = 0;
            }
                
            //Echo back the message that came in
            else
            {
                //set the string terminating NULL byte on the end
                //of the data read
                printf("ID %i: Valread:%i Byte1: %i, Byte2: %i, Byte3: %i, Byte4: %i, Byte5: %i\n\n",i,valread,buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
                buffer[valread] = '\0';
                
            }
        }
    }*/
}

int Server::readServer(){
    for (i = 0; i < max_clients; i++)
    {
        sd = client_socket[i];
            
        if (FD_ISSET( sd , &readfds))
        {
            char buffer[1025] = {'\0'};
            if ((valread = read( sd , buffer, 1024)) == 0)
            {
                //Somebody disconnected , get his details and print
                getpeername(sd , (struct sockaddr*)&address , \
                    (socklen_t*)&addrlen);
                printf("Host disconnected , ip %s , port %d \n" ,
                    inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                    
                //Close the socket and mark as 0 in list for reuse
                close( sd );
                
                client_socket[i] = 0;
            }
            else{
                printf("%s",buffer);
            }
        }
    }

}

int Server::writeServer(uint8_t msg){
    for (i = 0; i < max_clients; i++)
    {
        sd = client_socket[i];
            
        if (FD_ISSET( sd , &readfds))
        {
            char MessageBuffer[2] = {msg,'\0'};
            return send(sd , MessageBuffer, strlen(MessageBuffer),0);
        }
    }

}