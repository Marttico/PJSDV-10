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


#include "Chair.h"
#define TRUE 1
#define FALSE 0

    
int main(int argc , char *argv[])
{
    
    Server s1(8080);
    
    while(TRUE)
    {
       s1.setUp();
       s1.readServer();
       s1.writeServer(0x02);
       usleep(100000);
    }
        
    return 0;
}