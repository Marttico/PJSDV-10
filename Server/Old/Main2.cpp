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
    SocketClient cl(argv[1],8080);
    cl.setup();
    while(1){
        uint8_t buffer[64] = {0};
        cl.readSocketClient(buffer);
        printf("%i,%i\n",buffer[1],buffer[2]);
        
        uint16_t analog = ((buffer[1]&0x03) << 8) + buffer[2];
        //printf("%i\n",analog);
        /*for(uint8_t i = 0; i < 4; i++){
            printf("%i",i);
            cl.writeSocketClient(1<<i);
            usleep(100000);
        }*/
        //cl.writeSocketClient(0);
        //printf("%i\n",buffer[1]);
        //usleep(100000);
        if(analog > 600){
            cl.writeSocketClient(2);
        }else{
            cl.writeSocketClient(0);
        }

        /*cl.writeSocketClient(0b00001010);
        printf("On %i\n", i);
        usleep(100000);
        cl.writeSocketClient(0b00000101);
        printf("Off %i\n", i);
        usleep(100000);*/
        //printf("On %i, %i\n", cl.writeSocketClient(0x01), i);
        //usleep(100000);
    }
        
    return 0;
}