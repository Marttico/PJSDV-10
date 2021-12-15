
#include <iostream>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string> 
#include "Chair.h"
Chair::Chair(int port,bool trilPerms):sv(1,port),led(false),trilPermissie(trilPerms),buttonPressed(false),trilStand(false),drukSensor(0){

}

Chair::~Chair(){

}
void Chair::loop(){
    sv.loop();
    uint64_t temp;
    temp = sv.readServer();
    //printf("tempvalue: %i",temp);
    if(temp != 0){
        //sync output to variables
        buttonPressed = (temp & 0x01);
        //printf("BUtton: %i\n\n",buttonPressed);
        drukSensor = (temp >> 8) & 0x03FF;

        behaviour();
        
        //Maak bericht aan
        uint8_t msg = 0;
        //printf("Led: %i\n\n",led);
        msg = led << 4 || trilStand << 5;
        sv.writeServer(msg);
    }
}

void Chair::behaviour(){
    printf("%i",drukSensor);
    if(drukSensor > 600){
        zetTril(true);
    }else{
        zetTril(false);
    }
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