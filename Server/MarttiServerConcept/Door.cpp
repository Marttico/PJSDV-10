#include "Door.h"

Door::Door(int Port,int DoorOpenTimerDelay,string Prefix,CommandLineInput* CLI,  Column* colmn):
doorOpenTimerDelay(DoorOpenTimerDelay),doorAngle(70),prefix(Prefix),cli(CLI),port(Port),wm(Port),ledMode(false),inputButton(false),oldInputButton(inputButton),openPermissie(true),cl(colmn)
{}

Door::~Door(){}

//Behaviour
void Door::behaviour(){
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
        ledtimer = getMillis();
        zetDoorAngle(180);
        zetLed(true);
    }

    //If the difference between the current milliseconds since epoch and doortime is larger than 20000 milliseconds, close the door
    if(getMillis()-doortimer > doorOpenTimerDelay && getMillis()-doortimer < doorOpenTimerDelay+1000){
        zetDoorAngle(70);
    }
    if(getMillis()-ledtimer > doorOpenTimerDelay+2000 && getMillis()-ledtimer < doorOpenTimerDelay+3000){
        zetLed(false);
    }

    if(cl->brand)
    {
        zetDoorAngle(180);
    }

    //Format next message with object data
    char msg[1024] = {0};
    //The S flag tells the wemos there's a servo connected
    sprintf(msg,"%i,S,%i\r",((ledMode & 0x01) <<4),doorAngle);
    oldInputButton = inputButton;
    //Send data to the Wemos
    wm.writeWemos(msg);
}

//Commands
void Door::triggerCommands(){
    if(!(cli -> getExecuted())){
        //Wait for CLI to not be busy
        //Put commands below. The format is as follows commandCompare("<insert command here>",&Chair::<insertFunctionHere>,<insertValueIfCommandIsMet>,&executed);
        if(commandCompare(".ledaan")){zetLed(true);cli -> setExecuted();}
        if(commandCompare(".leduit")){zetLed(false);cli -> setExecuted();}

        if(commandCompare(".permaan")){zetOpenPermissie(true);cli -> setExecuted();}
        if(commandCompare(".permuit")){zetOpenPermissie(false);cli -> setExecuted();}



        if(commandCompare(".pushbutton")){zetDebugButton(true);cli -> setExecuted();}

        if(commandCompare(".opendoor")){zetDoorAngle(180);cli -> setExecuted();}
        if(commandCompare(".closedoor")){zetDoorAngle(70);cli -> setExecuted();}
        //commandCompare(".opendoor", &Door::zetDoorAngle,1000,&executed);
        //commandCompare(".closedoor", &Door::zetDoorAngle,2000,&executed);
    }
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
        inputButton = ((statusBits>>1) & 0x01) | (statusBits & 0x01);
    }
}

bool Door::commandCompare(string i){
    char temp[1024];
    //Add prefix to temp character array
    strcpy(temp,prefix.c_str());
    //Add command string to character array
    strcat(temp,i.c_str());
    //Compare input to temp
    return !strcmp((cli -> getCLI()).c_str(),temp);
}

uint64_t Door::getMillis(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

//Object Specific Functions
void Door::zetLed(bool i){
    ledMode = i;
}

void Door::zetDebugButton(bool i){
    inputButton = i;
}

void Door::zetOpenPermissie(bool i){
    openPermissie = i;
}

void Door::zetDoorAngle(int i){
    if(openPermissie){
        doorAngle = i;
    }
}
