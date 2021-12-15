
#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string> 
#include "Chair.h"
Chair::Chair(int port,bool trilPerms):sv(1,port),led(false),trilPermissie(trilPerms),buttonPressed(false),trilStand(false),drukSensor(0){

}
void Chair::loop(){
    sv.loop();
    sv.readServer();
    sv.writeServer();
}
void Chair::zetLed(bool i){
    led = i;
}
void Chair::zetTril(bool i){
    if(trilPermissie){
        trilStand = i;
    }else{
        trilStand = false;
    }
}
void Chair::zetTrilPermissie(bool i){
    trilPermissie = i;
}
bool Chair::checkTril() const{
    return trilStand;
}

uint16_t Chair::checkDrukSensor() const{
    return drukSensor;
}

bool Chair::checkKnop() const{
    return buttonPressed;
}