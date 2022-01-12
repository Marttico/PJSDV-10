#include "Column.h"

Column::Column(int Port, string Prefix, CommandLineInput* CLI): cli(CLI),prefix(Prefix), ledMode(false), zoemerMode(false), port(Port), wm(Port){}

Column::~Column(){}

//Behaviour
void Column::behaviour(){
    //while(1){
        //Read Wemos Status
        char readMessage[1024] = {0};
        wm.readWemos(readMessage);

        //Convert message to Object Attributes
        convertMessageToObjectAttr(readMessage);

        //Handle Command Line Commands
        triggerCommands();
        
        //Define behaviour of the object
        if(sensorwaarde > 500)
        {
            zetZoemer(true);
        }
        if(inputButton)
        {
            zetZoemer(false);
        }

        //Format next message with object data
        char msg[1024] = {0};
        sprintf(msg,"%i, ,%i\r",((zoemerMode & 0x01) << 4) + ((ledMode & 0x01) <<5),1023); /**TODO: hoe werkt dit bij zuil**/

        //Send data to the Wemos
        wm.writeWemos(msg);
    //}
}

//Commands
void Column::triggerCommands(){
    if(!(cli -> getExecuted())){
        //Put commands below. The format is as follows commandCompare("<insert command here>",&Chair::<insertFunctionHere>,<insertValueIfCommandIsMet>,&executed);
        if(commandCompare(".buzaan")){zetZoemer(true);cli -> setExecuted();}
        if(commandCompare(".buzuit")){zetZoemer(false);cli -> setExecuted();}
        if(commandCompare(".ledaan")){zetLed(true);cli -> setExecuted();}
        if(commandCompare(".leduit")){zetLed(false);cli -> setExecuted();}
    }
}

//Basic Functions
void Column::convertMessageToObjectAttr(char* msg)
{
    if(wm.isConnected() && msg[0] != 0)
    {
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
        sensorwaarde = analog1Bits;
        
    }

}

bool Column::commandCompare(string i){
    char temp[1024];
    //Add prefix to temp character array
    strcpy(temp,prefix.c_str());
    //Add command string to character array
    strcat(temp,i.c_str());
    //Compare input to temp
    return !strcmp((cli->getCLI()).c_str(),temp);
}

//Object Specific Functions
void Column::zetZoemer(bool i)
{
        zoemerMode = i;
}

void Column::zetLed(bool i)
{
        ledMode = i;
}
