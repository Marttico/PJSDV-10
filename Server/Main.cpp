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
    
    //Chair ch(8080,true);
    Server s1(1, 8080);
    //Server bed(1, 8081);
    char msg;
    while(TRUE)
    {
        uint8_t msg;
        if(!strcmp(argv[1],"1")){
            msg = 0b00100000;
        }else{
            msg = 0b00000000;
        }

        //printf("%i",argv[1]);
        //ch.loop();
        //ch.loop();
        s1.loop();
        //bed.loop();
        s1.readServer();
        //bed.readServer();
        s1.writeServer(msg);
        //bed.writeServer('d');
    }
        
    return 0;
}