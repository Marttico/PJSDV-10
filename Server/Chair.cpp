#include "Chair.h"

Chair::Chair(int Port,bool TrilPerms,string Prefix,CommandLineInput* CLI, ofstream& Bestand):prefix(Prefix),cli(CLI),port(Port),wm(Port),trilMode(true),trilPerms(TrilPerms),ledMode(true), bestand(Bestand),ddd(), tril(false),  hankIsOnChair(false){}

Chair::~Chair(){}

//Behaviour
void Chair::behaviour(){
    //while(1){
        //Read Wemos Status
        char readMessage[1024] = {0};
        wm.readWemos(readMessage);

        //Convert message to Object Attributes
        convertMessageToObjectAttr(readMessage);

        //Handle Command Line Commands
        triggerCommands();
        
        //Define behaviour of the object
        //Vibrate chair if inputPressure is bigger 600
        zetTril(inputPressure > 600);

        //Format next message with object data
        char msg[1024] = {0};
        sprintf(msg,"%i, ,%i\r",((trilMode & 0x01) << 5) + ((ledMode & 0x01) <<4),1023);

        //Detect whether the pressur of the pressure sensor is above 500
        if(inputPressure> 500)
        {   
             //Check if the variable hankIsOnChair is false
            if(!hankIsOnChair)
            {
                hankIsOnChair = true;
                cout<<"Hank zit"<<endl;
                //Write ":: hank zit op stoel" in log
                bestand<<ddd<<prefix<<":: hank zit op stoel"<<endl;
            }
        }
        //Detect whether the pressure of the pressure sensor is below 200
        if(inputPressure < 200)
        {
            //Check if the variable hankIsOnChair is true
            if(hankIsOnChair)
            {
                hankIsOnChair = false;
                cout<<"Hank staat op van de stoel"<<endl;
                //Write ":: hank is opgestaan van de stoel" in log
                bestand<<ddd<<prefix<<":: hank is opgestaan van de stoel"<<endl;
            }
        }
        //Send data to the Wemos
        wm.writeWemos(msg);
    //}
}

//Commands
void Chair::triggerCommands(){
    //Check whether the latest command in the commandline has already been executed
    if(!(cli -> getExecuted())){
        //Syntax: if(commandCompare(".<Insert Command here>")){<Insert Function here>;cli -> setExecuted();}
        //This will trigger the specified function when the command is detected.
        if(commandCompare(".trilaan")){zetTril(true);cli -> setExecuted();}
        if(commandCompare(".triluit")){zetTril(false);cli -> setExecuted();}
        if(commandCompare(".ledaan")){zetLed(true);cli -> setExecuted();}
        if(commandCompare(".leduit")){zetLed(false);cli -> setExecuted();}
        if(commandCompare(".trilpermaan")){zetTrilPermissie(true);cli -> setExecuted();}
        if(commandCompare(".trilpermuit")){zetTrilPermissie(false);cli -> setExecuted();}
    }
}

//Basic functions
void Chair::convertMessageToObjectAttr(char* msg){
    //Check if this object is connected. Since we're unable to read messages if there's nothing to read from...
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

//Object Specific Functions
void Chair::zetTril(bool i){
    //If the chair object is allowed to "tril", let it "tril", otherwise, do not.
    if(trilPerms){
        trilMode = i;
    }else{
        trilMode = false;
    }
}

void Chair::zetTrilPermissie(bool i){
    //Change private variable to argument
    trilPerms = i;

    //Updating trilMode
    zetTril(trilMode);
}

void Chair::zetLed(bool i){
    //Change private variable to argument
    ledMode = i;
}
