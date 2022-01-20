#include "Wemos.h"

Wemos::Wemos(int Port):port(Port),wemosServer(Port){

}

Wemos::~Wemos(){

}

int Wemos::writeWemos(char* msg){
    char sendBuf[1024] = "W,";
    //Add message character array to sendBuf
    strcat(sendBuf,msg);

    //Send write command to the wemos containing status data
    int returnValue = wemosServer.sendMessage(sendBuf);
    return returnValue;
}

int Wemos::readWemos(char* msg){
    //Send a read command to the wemos
    char buf[1024] = "R\r";
    wemosServer.sendMessage(buf);
    usleep(senddelay * 2);

    //Read message (probably containing status of the wemos module)
    msg[0] = '\0';
    int returnValue = wemosServer.readMessage(msg);
    //usleep(senddelay);
    return returnValue;
}

//Return whether the Wemos is connected
bool Wemos::isConnected() const{
    return wemosServer.isConnected();
}