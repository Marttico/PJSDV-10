#include "Bed.h"

Bed::Bed(int Port,string Prefix,CommandLineInput* CLI, ofstream& Bestand):prefix(Prefix),cli(CLI),port(Port),wm(Port),ledMode(false), bestand(Bestand),ddd(),inputPressure(0), hankIsOnBed(false){}

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
    //Detect positive change in button
    if(inputButton > oldInputButton){
        //Write "::knopingedrukt" in log
        bestand << ddd << prefix << "::knopingedrukt" << endl;
        //Check if the LED is not on
        if(!ledMode){
            //Turn on LED
            zetLed(true);
            //Set bedTimer variable to current epoch time.
            bedTimer = getMillis();
            
        }else{
            //Turn off LED
            zetLed(false);
            //Reset the bedTimer variable
            bedTimer = 0;
        }
    }

    //Detect whether the pressur of the pressure sensor is above 500
    if(inputPressure > 500)
    {
        //Check if the variable hankIsOnBed is false
        if(!hankIsOnBed)
        {
            hankIsOnBed = true;
            cout<<"Hank slaapt"<<endl;
            //Write ":: hank ligt op bed" in log
            bestand<<ddd<<prefix<<":: hank ligt op bed"<<endl;
        }
    }
    //Detect whether the pressure of the pressure sensor is below 200
    if(inputPressure < 200)
    {
        //Check if the variable hankIsOnBed is true
        if(hankIsOnBed)
        {
            hankIsOnBed = false;
            cout<<"Hank staat op"<<endl;
            //Write ":: hank is opgestaan" in log
            bestand<<ddd<<prefix<<":: hank is opgestaan"<<endl;
        }
    }

    //The LED turns off after 10000 milliseconds. The program compares the current time to the time the button got pressed.
    if(getMillis() - bedTimer > 10000 && getMillis() - bedTimer < 11000){
        //Turn off LED
        zetLed(false);
        //Write "::ledtimeout" in log
        bestand << ddd << prefix << "::ledtimeout" << endl;
    }

    //Format next message with object data
    char msg[1024] = {0};
    sprintf(msg,"%i, ,%i\r",((ledMode & 0x01) << 4),1023);

    //Send data to the Wemos
    wm.writeWemos(msg);

    //Update old variables for next cycle
    oldInputButton = inputButton;
}

//Commands
void Bed::triggerCommands(){
    //Check whether the latest command in the commandline has already been executed
    if(!(cli -> getExecuted())){
        //Syntax: if(commandCompare(".<Insert Command here>")){<Insert Function here>;cli -> setExecuted();}
        //This will trigger the specified function when the command is detected.
        if(commandCompare(".ledaan")){zetLed(true);cli -> setExecuted();}
        if(commandCompare(".leduit")){zetLed(false);cli -> setExecuted();}
    }
}

//Basic Functions
void Bed::convertMessageToObjectAttr(char* msg){
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
    //Return milliseconds since Epoch (1 Jan 1970)
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

//Object Specific Functions
void Bed::zetLed(bool i){
    //Change private variable to argument
    ledMode = i;
}

int Bed::InputPressure() const{
    //Return private variable inputPressure
    return inputPressure;
}