#include "zuil.h"

Zuil::Zuil(int Port, string Prefix, string* CommandLine): prefix(Prefix), commandLine(CommandLine), ledMode(false), zoemerMode(false), port(Port), wm(Port), th(&Zuil::behaviour, this)
{
    //ctor
}

Zuil::~Zuil()
{
    //dtor
}

Zuil::Zuil(const Zuil& other)
{
    //copy ctor
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
        sprintf(msg,"%i,%i\r",((zoemerMode) & 0x01) << 4) + ((ledMode & 0x01) <<5),1023); /**TODO: hoe werkt dit bij zuil**/

        //Send data to the Wemos
        wm.writeWemos(msg);
    }
}

bool Zuil::convertMessageToObjectAttr()
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
        gasSensor = /**TODO: waar zit gassensor?**/
        //inputPressure = analog0Bits & 0x03FF;//0x03FF = 0b0000001111111111
    }

}
/**todo: commandline **/

