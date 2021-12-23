#include "Bed.h"

Bed::Bed(int Port,string Prefix,CommandLineInput* CLI):prefix(Prefix),cli(CLI),port(Port),wm(Port),ledMode(true),th(&Bed::behaviour,this){}

Bed::~Bed(){}

//Behaviour
void Bed::behaviour(){
    //Read Wemos Status
    char readMessage[1024] = {0};
    wm.readWemos(readMessage);
    
    //Convert message to Object Attributes
    convertMessageToObjectAttr(readMessage);

    //Handle Command Line Commands
    triggerCommands();
    
    //Define behaviour of the object
    if(inputButton){
        zetLed(true);
        bedTimer = getMillis();
    }

    //The led turns off after 10000 miliseconds if the difference is bigger
    if(getMillis() - bedTimer > 10000 && getMillis() - bedTimer < 11000){
        zetLed(false);
    }

    //Format next message with object data
    char msg[1024] = {0};
    sprintf(msg,"%i, ,%i\r",((ledMode & 0x01) << 4),1023);

    //Send data to the Wemos
    wm.writeWemos(msg);
}

//Commands
bool Bed::triggerCommands(){
    bool executed = false;
    //Wait for CLI to not be busy
    //while(cli -> checkBusy());
    cli -> setBusy(true);
    
    //Put commands below. The format is as follows commandCompare("<insert command here>",&Chair::<insertFunctionHere>,<insertValueIfCommandIsMet>,&executed);
    if(commandCompare(".ledaan")){zetLed(true);executed = true; cli ->clearCLI();}
    if(commandCompare(".leduit")){zetLed(false);executed = true; cli ->clearCLI();}
    cli -> setBusy(false);
    return executed;
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

bool Bed::commandCompare(string i){
    char temp[1024];
    //Add prefix to temp character array
    strcpy(temp,prefix.c_str());
    //Add command string to character array
    strcat(temp,i.c_str());
    //Compare input to temp
    return !strcmp((cli->getCLI()).c_str(),temp);
}

uint64_t Bed::getMillis(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

//Object Specific Functions
void Bed::zetLed(bool i){
    ledMode = i;
}
