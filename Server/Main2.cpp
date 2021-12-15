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

#include "SocketClient.h"
#define TRUE 1
#define FALSE 0

    
int main(int argc , char *argv[])
{
    SocketClient cl("192.168.2.22",8080);
    cl.setup();
    while(TRUE)
    {
        cl.writeSocketClient(0b00100000);
        sleep(1);
        cl.writeSocketClient(0b00000000);
        sleep(1);
    }
        
    return 0;
}