#include "Wemos.h"

Wemos::Wemos(int Port):port(Port),wemosServer(Port){

}
Wemos::~Wemos(){

}
int Wemos::writeWemos(char* msg){
    char sendBuf[1024] = "W,";
    strcat(sendBuf,msg);
    //printf("Sending Write Command\n");
    int returnValue = wemosServer.sendMessage(sendBuf);
    usleep(senddelay);
    return returnValue;
}

int Wemos::readWemos(char* msg){
    char buf[1024] = "R\r";
    //printf("Sending Read Command\n");
    wemosServer.sendMessage(buf);
    usleep(senddelay);
    msg[0] = '\0';
    //printf("Reading Wemos Status\n");
    int returnValue = wemosServer.readMessage(msg);
    usleep(senddelay);
    return returnValue;
    
}