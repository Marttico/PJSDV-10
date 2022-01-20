#include "Door.h"

Door::Door(int Port,int DoorOpenTimerDelay,string Prefix,CommandLineInput* CLI, ofstream& Bestand):
doorOpenTimerDelay(DoorOpenTimerDelay),doorAngle(70),prefix(Prefix),cli(CLI),port(Port),wm(Port),ledMode(false),inputButton(false),oldInputButton(inputButton),openPermissie(true),cl(NULL),bed(NULL), bestand(Bestand),ddd(), ledLogTimer(false)
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

    //Default state of LED
    zetLed(false);

    //Define behaviour of the object
    //Detect positive change in inputButton
    if(inputButton > oldInputButton){
        //Write "::De deurknop is ingedrukt." in log
        bestand << ddd << prefix << "::De deurknop is ingedrukt." << endl;

        //Set doortimer to current epoch time
        doortimer = getMillis();
        //Set ledtimer to current epoch time
        ledtimer = getMillis();
        //Attempt to open door (180 degrees = opened)
        zetDoorAngle(180);
        //Turn on LED
        zetLed(true);
        ledLogTimer = false;
    }

    //If the difference between the current milliseconds since epoch and doortime is larger than the specified doorOpenTimerDelay + 1000 milliseconds, close the door
    if(getMillis()-doortimer > doorOpenTimerDelay && getMillis()-doortimer < doorOpenTimerDelay+1000 && doorAngle != 70){
        //Attempt to close door (70 degrees = closed)
        zetDoorAngle(70);
        //Write "::deur is automatisch dicht gegaan." in log
        bestand << ddd << prefix << "::deur is automatisch dicht gegaan." << endl;

    }

    //If the difference between the current milliseconds since epoch and ledtimer is larger than the specified doorOpenTimerDelay + 2000 milliseconds, close the door
    if(getMillis()-ledtimer < doorOpenTimerDelay+2000 && !ledMode){
        if(!ledLogTimer)
        {
            //Write "::ledje staat aan." in log
            bestand << ddd << prefix << "::ledje staat aan." << endl;
            ledLogTimer = true;
        }
        zetLed(true);
    }
    
    //If the difference between the current milliseconds since epoch and ledtimer is between the specified doorOpenTimerDelay + 2000 milliseconds and doorOpenTimerDelay + 3000, close the door
    if(getMillis()-ledtimer > doorOpenTimerDelay+2000 && getMillis()-ledtimer < doorOpenTimerDelay+3000 && !ledMode){
        if(ledLogTimer)
        {
            bestand << ddd << prefix << "::ledje staat uit." << endl;
            ledLogTimer = false;
        }
        //Turn off LED
        zetLed(false);
    }


    if(cl->isBrand())
    {
        //Attempt to open door
        zetDoorAngle(180);
    }
    if(!openPermissie){
        //Attempt to close door
        zetDoorAngle(70);
    }

    //Check if bed is connected
    if(bed != NULL){
        //Get bed info
        //Detect bed inputPressure
        if(bed->InputPressure() > 600){
            //Turn LED on
            zetLed(true);
        }
    }

    //Format next message with object data
    char msg[1024] = {0};
    //The S flag tells the wemos there's a servo connected
    sprintf(msg,"%i,S,%i\r",((ledMode & 0x01) <<4),doorAngle);

    //Send data to the Wemos
    wm.writeWemos(msg);

    //Update old variables for next cycle
    oldInputButton = inputButton;
}

//Commands
void Door::triggerCommands(){
    //Check whether the latest command in the commandline has already been executed
    if(!(cli -> getExecuted())){
        //Syntax: if(commandCompare(".<Insert Command here>")){<Insert Function here>;cli -> setExecuted();}
        //This will trigger the specified function when the command is detected.
        if(commandCompare(".ledaan")){zetLed(true);cli -> setExecuted();}
        if(commandCompare(".leduit")){zetLed(false);cli -> setExecuted();}
        if(commandCompare(".permaan")){zetOpenPermissie(true);cli -> setExecuted();}
        if(commandCompare(".permuit")){zetOpenPermissie(false);cli -> setExecuted();}
        if(commandCompare(".pushbutton")){zetDebugButton(true);cli -> setExecuted();}
        if(commandCompare(".opendoor")){zetDoorAngle(180);cli -> setExecuted();}
        if(commandCompare(".closedoor")){zetDoorAngle(70);cli -> setExecuted();}
    }
}

//Basic Functions
void Door::convertMessageToObjectAttr(char* msg){
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
    //Return milliseconds since Epoch (1 Jan 1970)
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

//Object Specific Functions
void Door::zetLed(bool i){
    //Change private variable to argument
    ledMode = i;
}

//Add relationship between Door and Bed
void Door::add(Bed* bedInput){
    //Assign pointer bed the address of the argument bedInput
    bed = bedInput;
}

//Add relationship between Door and Column
void Door::add(Column* columnInput){
    //Assign pointer cl the address of the argument columnInput
    cl = columnInput;
}

void Door::zetDebugButton(bool i){
    //Change private variable to argument
    inputButton = i;
}

void Door::zetOpenPermissie(bool i){
    //Change private variable to argument
    openPermissie = i;
}

void Door::zetDoorAngle(int i){
    //Check if the door is allowed to open
    if(openPermissie){
        //Change private variable to argument
        doorAngle = i;
        //if i is bigger than 70, and there is no fire, print "Deur is geopend" in console
        if(i >70)
        {
            if(!cl->isBrand())
                cout<<"Deur is geopend"<<endl;
        }
        //if i is = 70, and there is no fire, print "Deur is gesloten" in console
        else if(i == 70)
        {
            cout<<"Deur is gesloten" <<endl;
        }
    }
    //Check if there is a fire detected
    else if (cl->isBrand()){
        //Open door
        doorAngle = 180;
    }
    //If the door is not allowed to open and there is no fire, close the door.
    else{
        //Close door
        doorAngle = 70;
    }

}
