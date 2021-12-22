#include "Column.h"

Column::Column(int Port, string Prefix, CommandLineInput* CLI): cli(CLI),prefix(Prefix), ledMode(false), zoemerMode(false), port(Port), wm(Port), th(&Column::behaviour, this)
{
    //ctor
}

Column::~Column()
{
    //dtor
}


void Column::behaviour()
{
    //while(true)
    //{
        char readMessage[1024] = {0};
        wm.readWemos(readMessage);

        //Convert message to Object Attributes
        convertMessageToObjectAttr(readMessage);

        //Handle Command Line Commands
        triggerCommands();
        //printf("%i\n",sensorwaarde);
        //Define behaviour of the object
        if(sensorwaarde >550 )
        {
                zetZoemer(true);
                
        }
        if(inputButton)
        {
                zetZoemer(false);
        }

        //Format next message with object data
        char msg[1024] = {0};
        /*
        //Check de byte offset positions op de Chairs enzo.
        */
        sprintf(msg,"%i, ,%i\r",((zoemerMode & 0x01) << 4) + ((ledMode & 0x01) <<5),1023); /**TODO: hoe werkt dit bij zuil**/

        //Send data to the Wemos
        wm.writeWemos(msg);
    //}
}

void Column::convertMessageToObjectAttr(char* msg)
{
    if(wm.isConnected() && msg[0] != 0)
    {
        //printf("%s\n",msg);
        
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


bool Column::triggerCommands(){
    bool executed = false;
    //Wait for CLI to not be busy
    //while(cli -> checkBusy());
    cli -> setBusy(true);
    //Put commands below. The format is as follows commandCompare("<insert command here>",&Chair::<insertFunctionHere>,<insertValueIfCommandIsMet>,&executed);
    if(commandCompare(".buzaan")){zetZoemer(true);executed = true; cli -> clearCLI();}
    if(commandCompare(".buzuit")){zetZoemer(false);executed = true; cli -> clearCLI();}
    if(commandCompare(".ledaan")){zetLed(true);executed = true; cli -> clearCLI();}
    if(commandCompare(".leduit")){zetLed(false);executed = true; cli -> clearCLI();}
    cli -> setBusy(false);
    return executed;
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

void Column::zetZoemer(bool i)
{
        zoemerMode = i;
}

void Column::zetLed(bool i)
{
        ledMode = i;
}
