#include "zuil.h"

Zuil::Zuil(int Port, string Prefix, string* CommandLine): prefix(Prefix), commandLine(CommandLine), ledMode(false), zoemerMode(false), port(Port), wm(Port), th(&Zuil::behaviour, this)
{
    //ctor
}

Zuil::~Zuil()
{
    //dtor
}


void Zuil::behaviour()
{
    while(true)
    {
        char readMessage[1024] = {0};
        wm.readWemos(readMessage);

        //Convert message to Object Attributes
        convertMessageToObjectAttr(readMessage);

        //Handle Command Line Commands
        triggerCommands();

        //Define behaviour of the object
        //zetTril(inputPressure > 600);

        //Format next message with object data
        char msg[1024] = {0};
        /*
        //Check de byte offset positions op de Chairs enzo.
        */
        sprintf(msg,"%i,%i\r",((zoemerMode & 0x01) << 4) + ((ledMode & 0x01) <<5),1023); /**TODO: hoe werkt dit bij zuil**/

        //Send data to the Wemos
        wm.writeWemos(msg);
    }
}

void Zuil::convertMessageToObjectAttr(char* msg)
{
    if(wm.isConnected() && msg[0] != 0)
    {
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
        
        if(analog1Bits >550 )
        {
                //gasSensor = 1;
                zetZoemer(true);
                
        }
        
        else
        {
                //zetZoemer(false);
                //alarm = true;
        }
        if(inputButton)
        {
                zetZoemer(false);
        }
        
        //zetZoemer(alarm);
        //cout<<analog1Bits<<endl;
        //gasSensor = analog1Bits & 0x03FF;/**TODO: waar zit gassensor?**/
        //inputPressure = analog0Bits & 0x03FF;//0x03FF = 0b0000001111111111
    }

}
/**todo: commandline **/

bool Zuil::triggerCommands(){
    bool executed = false;
    
    //Put commands below. The format is as follows commandCompare("<insert command here>",&Chair::<insertFunctionHere>,<insertValueIfCommandIsMet>,&executed);
   
    commandCompare(".buzaan", &Zuil::zetZoemer,true,&executed);
    commandCompare(".buzuit", &Zuil::zetZoemer,false,&executed);
    commandCompare(".ledaan", &Zuil::zetLed,true,&executed);
   commandCompare(".leduit", &Zuil::zetLed,false,&executed);
    return executed;
}
void Zuil::commandCompare(string i, void (Zuil::*func)(bool), bool mode, bool* exec){
    char temp[1024];
    strcpy(temp,prefix.c_str());
    strcat(temp,i.c_str());
    if(!strcmp((*commandLine).c_str(),temp)){
        (this ->*func)(mode);
        *exec = true;
        (*commandLine)[0] = 0;
    }
}
void Zuil::zetZoemer(bool i)
{
        
        zoemerMode = i;
        //alarm = i;
}
void Zuil::zetLed(bool i)
{
        ledMode = i;
}
