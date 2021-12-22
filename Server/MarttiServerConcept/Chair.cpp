#include "Chair.h"

Chair::Chair(int Port,bool TrilPerms,string Prefix,CommandLineInput* CLI):prefix(Prefix),cli(CLI),port(Port),wm(Port),trilMode(true),trilPerms(TrilPerms),ledMode(true),th(&Chair::behaviour,this){

}

Chair::~Chair(){

}

void Chair::behaviour(){
    //while(true){
        //Read Wemos Status
        char readMessage[1024] = {0};
        wm.readWemos(readMessage);
        
        //Convert message to Object Attributes
        convertMessageToObjectAttr(readMessage);

        //Handle Command Line Commands
        triggerCommands();
        
        //Define behaviour of the object
        //If the inputpressure on the chair is higher than 600, turn on the "tril"
        zetTril(inputPressure > 600);

        //Format next message with object data
        char msg[1024] = {0};
        /*
        //Check de byte offset positions op de Chairs enzo.
        */
        sprintf(msg,"%i, ,%i\r",((trilMode & 0x01) << 5) + ((ledMode & 0x01) <<4),1023);

        //Send data to the Wemos
        wm.writeWemos(msg);
    //}
}

bool Chair::triggerCommands(){
    bool executed = false;
    
    //Wait for CLI to not be busy
    //while(cli -> checkBusy());
    cli -> setBusy(true);
    
    //Put commands below. The format is as follows commandCompare("<insert command here>",&Chair::<insertFunctionHere>,<insertValueIfCommandIsMet>,&executed);
    if(commandCompare(".trilaan")){zetTril(true);executed = true; cli ->clearCLI();}
    if(commandCompare(".triluit")){zetTril(false);executed = true; cli ->clearCLI();}
    if(commandCompare(".ledaan")){zetLed(true);executed = true; cli ->clearCLI();}
    if(commandCompare(".leduit")){zetLed(false);executed = true; cli ->clearCLI();}
    if(commandCompare(".trilpermaan")){zetTrilPermissie(true);executed = true; cli ->clearCLI();}
    if(commandCompare(".trilpermuit")){zetTrilPermissie(false);executed = true; cli ->clearCLI();}
    cli -> setBusy(false);
    
    return executed;
}

void Chair::zetTril(bool i){
    //If the chair object is allowed to "tril", let it "tril", otherwise, do not.
    if(trilPerms){
        trilMode = i;
    }else{
        trilMode = false;
    }
}

void Chair::zetTrilPermissie(bool i){
    trilPerms = i;

    //Updating trilMode
    zetTril(trilMode);
}

void Chair::zetLed(bool i){
    ledMode = i;
}

//Basic Functions
void Chair::convertMessageToObjectAttr(char* msg){
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

bool Chair::commandCompare(string i){
    char temp[1024];
    //Add prefix to temp character array
    strcpy(temp,prefix.c_str());
    //Add command string to character array
    strcat(temp,i.c_str());
    //Compare input to temp
    
    
    return !strcmp((cli -> getCLI()).c_str(),temp);
}

/*
void Chair::commandCompare(string i, void (Chair::*func)(bool), bool mode, bool* exec){
    char temp[1024];
    //Add prefix to temp character array
    strcpy(temp,prefix.c_str());
    //Add command string to character array
    strcat(temp,i.c_str());
    //Compare input to temp
    
    
    if(!strcmp((*commandLine).c_str(),temp)){
        //Execute parameter function func
        (this ->*func)(mode);
        //Set exec to true
        *exec = true;
        //Clear commandline string
        (*commandLine)[0] = 0;
    }
}*/