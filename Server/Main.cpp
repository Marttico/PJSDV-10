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

#include "Server.h"
#define TRUE 1
#define FALSE 0

    
int main(int argc , char *argv[])
{
    
    
    Server s1(1, 8080);
    Server bed(1, 8081);
    char msg;
    while(TRUE)
    {

        s1.loop();
        bed.loop();
        s1.readServer();
        bed.readServer();
        s1.writeServer('m');
        bed.writeServer('d');
    }
        
    return 0;
}