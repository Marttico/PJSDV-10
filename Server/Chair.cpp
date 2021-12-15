#include "Chair.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string> 

Chair::Chair(int port,bool trilPerms):sv(1,port),trilPermissie(trilPerms),buttonPressed(false),trilStand(false),drukSensor(0){

}
void Chair::loop(){

}
void Chair::zetLed(bool i){

}
void Chair::zetTril(bool i){
    
}
void Chair::zetTrilPermissie(bool){

}
bool checkTril() const{
    return trilStand;
}

uint16_t checkDrukSensor() const{
    return drukSensor;
}

bool checkKnop() const{
    return buttonPressed;
}