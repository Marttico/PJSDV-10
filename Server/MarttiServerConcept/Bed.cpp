#include "Bed.h"

Bed::Bed(int Port,string Prefix,string* CommandLine):prefix(Prefix),commandLine(CommandLine),port(Port),wm(Port),ledMode(true),th(&Bed::behaviour,this){

}

Bed::~Bed(){

}

void Bed::behaviour(){
    while(true){
        //Read Wemos Status
        char readMessage[1024] = {0};
        wm.readWemos(readMessage);
        
        //Convert message to Object Attributes
        convertMessageToObjectAttr(readMessage);

        //Handle Command Line Commands
        triggerCommands();
        
        //Define behaviour of the object
        

        //Format next message with object data
        char msg[1024] = {0};
        /*
        //Check de byte offset positions op de Chairs enzo.
        */
        sprintf(msg,"%i,,%i\r",((ledMode & 0x01) <<5),1023);

        //Send data to the Wemos
        wm.writeWemos(msg);
    }
}

bool Bed::triggerCommands(){
    bool executed = false;
    
    //Put commands below. The format is as follows commandCompare("<insert command here>",&Chair::<insertFunctionHere>,<insertValueIfCommandIsMet>,&executed);
    commandCompare(".ledaan", &Bed::zetLed,true,&executed);
    commandCompare(".leduit", &Bed::zetLed,false,&executed);
    return executed;
}

void Bed::zetLed(bool i){
    ledMode = i;
}

//Basic Functions
void Bed::convertMessageToObjectAttr(char* msg){
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

        inputPressure = analog0Bits & 0x03FF;//0x03FF = 0b0000001111111111
    }
}

void Bed::commandCompare(string i, void (Bed::*func)(bool), bool mode, bool* exec){
    char temp[1024];
    strcpy(temp,prefix.c_str());
    strcat(temp,i.c_str());
    if(!strcmp((*commandLine).c_str(),temp)){
        (this ->*func)(mode);
        *exec = true;
        (*commandLine)[0] = 0;
    }
}