#include "Chair.h"

Chair::Chair(int Port,bool TrilPerms):port(Port),wm(Port),trilMode(true),trilPerms(TrilPerms),ledMode(true){

}

Chair::~Chair(){

}

void Chair::behaviour(){
    char readMessage[1024] = {0};
    wm.readWemos(readMessage);
    //Process the inputs and change outputs to the corresponding things idk
    /*
    // Here we'll add the actual behaviour of the chair to correspond to what it's supposed to be.
    */
    //Format the data into a message the Wemos can understand
    char msg[1024] = {0};
    sprintf(msg,"%i,%i\r",((trilMode & 0x01) << 4) + ((ledMode & 0x01) <<5),1023);
    //Send data to the Wemos.
    wm.writeWemos(msg);
}

void Chair::zetTril(bool i){
    if(trilPerms){
        trilMode = i;
    }else{
        trilMode = false;
    }
}

void Chair::zetTrilPermissie(bool i){
    trilPerms = i;
}

void Chair::zetLed(bool i){
    ledMode = i;
}