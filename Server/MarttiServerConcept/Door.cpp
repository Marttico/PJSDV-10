#include "Door.h"

Door::Door(int Port,string Prefix,string* CommandLine):doorAngle(0),prefix(Prefix),commandLine(CommandLine),port(Port),wm(Port),ledMode(false),inputButton(false),oldInputButton(inputButton),th(&Door::behaviour,this){

}

Door::~Door(){

}

void Door::behaviour(){
    while(true){
        //Read Wemos Status
        char readMessage[1024] = {0};
        wm.readWemos(readMessage);
        
        //Convert message to Object Attributes
        convertMessageToObjectAttr(readMessage);

        //Handle Command Line Commands
        triggerCommands();
        
        //Define behaviour of the object
        //If there's a positive change in inputButton, open the door and set doortimer to the current milliseconds since epoch
        if(inputButton > oldInputButton){
            doortimer = getMillis();
            zetDoorAngle(90);
        }

        //If the difference between the current milliseconds since epoch and doortime is larger than 20000 milliseconds, close the door
        if(getMillis()-doortimer > 2000){
            zetDoorAngle(0);
        }

        //Format next message with object data
        char msg[1024] = {0};
        //The S flag tells the wemos there's a servo connected
        sprintf(msg,"%i,S,%i\r",((ledMode & 0x01) <<5),doorAngle);
        oldInputButton = inputButton;
        //Send data to the Wemos
        wm.writeWemos(msg);
    }
}


bool Door::triggerCommands(){
    bool executed = false;
    
    //Put commands below. The format is as follows commandCompare("<insert command here>",&Chair::<insertFunctionHere>,<insertValueIfCommandIsMet>,&executed);
    commandCompare(".ledaan", &Door::zetLed,true,&executed);
    commandCompare(".leduit", &Door::zetLed,false,&executed);

    commandCompare(".pushbutton", &Door::zetDebugButton,true,&executed);

    commandCompare(".opendoor", &Door::zetDoorAngle,1000,&executed);
    commandCompare(".closedoor", &Door::zetDoorAngle,2000,&executed);
    return executed;
}

void Door::zetLed(bool i){
    ledMode = i;
}

void Door::zetDebugButton(bool i){
    inputButton = i;
}

void Door::zetDoorAngle(int i){
    doorAngle = i;
}

//Basic Functions
void Door::convertMessageToObjectAttr(char* msg){
    if(wm.isConnected() && msg[0] != 0){
        
        //Get first element of message
        char *token = strtok(msg, ",");
        uint8_t statusBits = atoi(token);

        //Get second element of message
        token = strtok(NULL, ",");
        uint16_t analog0Bits = atoi(token);

        //Get third element of message
        token = strtok(NULL, ",");  
        uint16_t analog1Bits = atoi(token);

        //Set variables of object
        inputButton = statusBits & 0x01;
    }
}

void Door::commandCompare(string i, void (Door::*func)(bool), bool mode, bool* exec){
    char temp[1024];
    strcpy(temp,prefix.c_str());
    strcat(temp,i.c_str());
    if(!strcmp((*commandLine).c_str(),temp)){
        (this ->*func)(mode);
        *exec = true;
        (*commandLine)[0] = 0;
    }
}

void Door::commandCompare(string i, void (Door::*func)(int), int mode, bool* exec){
    char temp[1024];
    strcpy(temp,prefix.c_str());
    strcat(temp,i.c_str());
    if(!strcmp((*commandLine).c_str(),temp)){
        (this ->*func)(mode);
        *exec = true;
        (*commandLine)[0] = 0;
    }
}
uint64_t Door::getMillis(){
return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}