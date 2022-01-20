#include "Column.h"

Column::Column(int Port, int bw, string Prefix, CommandLineInput* CLI, ofstream& Bestand): cli(CLI),prefix(Prefix), ledMode(false), zoemerMode(false), port(Port), wm(Port), bestand(Bestand),ddd(),brandwaarde(bw), brand(false), brandTemp(brandwaarde){}

Column::~Column(){}

//Behaviour
void Column::behaviour(){
    //Read Wemos Status
    char readMessage[1024] = {0};
    wm.readWemos(readMessage);

    //Convert message to Object Attributes
    convertMessageToObjectAttr(readMessage);

    //Handle Command Line Commands
    triggerCommands();

    //Define behaviour of the object
    //Detect whether sensorwaarde is higher than brandwaarde
    if(sensorwaarde > brandwaarde && !brand)
    {
        //Start pulsing Raspberry Pi LED
        pl -> startFlashing();
        //Turn on speaker
        zetZoemer(true);
        brand = true;
        //Write "::er is brand, het alarm gaat aan!" in log
        bestand << ddd << prefix << "::er is brand, het alarm gaat aan!" << endl;
        //Print BRAND message in ASCII art
        cout << "  ____  _____            _   _ _____  " << endl << " |  _ \\|  __ \\     /\\   | \\ | |  __ \\ " << endl << " | |_) | |__) |   /  \\  |  \\| | |  | |" << endl << " |  _ <|  _  /   / /\\ \\ | . ` | |  | |" << endl << " | |_) | | \\ \\  / ____ \\| |\\  | |__| |" << endl <<" |____/|_|  \\_\\/_/    \\_\\_| \\_|_____/ " << endl;
    }
    

    if(sensorwaarde && brand) //als er brand is
    {
        brandTemp = brandwaarde; //sla huidige trigger waarde op
        brandwaarde = sensorwaarde; //pas de triggerwaarde aan op de huidge waarde. Hierdoor gaat het brandalarm niet meer af als de knop wordt ingedrukt, maar de brandsensor nog rook meld


    }
    if(!brand)
    {
        brandwaarde = brandTemp; //zet de triggerwaarde weer terug
    }

    //Detect whether the button on the Column has been pushed
    if(inputButton)
    {
        if(brand){
            brand = false;
            //Turn off speaker
            zetZoemer(false);
            //Stop pulsing Raspberry Pi LED
            pl -> stopFlashing();
            //Write "::het alarm is gestopt!" in log
            bestand << ddd << prefix << "::het alarm is gestopt!" << endl;
        }
    }

    //Format next message with object data
    char msg[1024] = {0};
    sprintf(msg,"%i, ,%i\r",((zoemerMode & 0x01) << 4) + ((ledMode & 0x01) <<5),1023); /**TODO: hoe werkt dit bij zuil**/

    //Send data to the Wemos
    wm.writeWemos(msg);
}

//Commands
void Column::triggerCommands(){
    //Check whether the latest command in the commandline has already been executed
    if(!(cli -> getExecuted())){
        //Syntax: if(commandCompare(".<Insert Command here>")){<Insert Function here>;cli -> setExecuted();}
        //This will trigger the specified function when the command is detected.
        if(commandCompare(".buzaan")){zetZoemer(true);cli -> setExecuted();}
        if(commandCompare(".buzuit")){zetZoemer(false);cli -> setExecuted();}
        if(commandCompare(".ledaan")){zetLed(true);cli -> setExecuted();}
        if(commandCompare(".leduit")){zetLed(false);cli -> setExecuted();}
        if(commandCompare(".printvalue")){printValue();cli -> setExecuted();}
    }
}

//Basic Functions
void Column::convertMessageToObjectAttr(char* msg)
{
    //Check if this object is connected. Since we're unable to read messages if there's nothing to read from...
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
    //Change private variable to argument
    zoemerMode = i;
}

void Column::zetLed(bool i)
{
    //Change private variable to argument
    ledMode = i;
}

int Column::isBrand() const
{
    //Return private variable brand
    return brand;
}

//Add relationship between Column and piLed
void Column::add(piLed* piled){
    //Assign pointer pl the address of the argument piLed
    pl = piled;
}

//Prints value of sensor in Console and Log
void Column::printValue(){
    //Print value of sensor in Log
    bestand << ddd << prefix << "::BrandSensor: " << sensorwaarde << endl;
    //Print value of sensor in Console
    cout <<"BrandSensor: "<< sensorwaarde << endl;
}
