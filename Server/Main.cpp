#include <iostream>
#include <string.h>
#include <thread>
#include <math.h>
#include <csignal>

#include "CommandLineInput.h"
#include "Chair.h"
#include "Door.h"
#include "Column.h"
#include "Bed.h"
#include "piLed.h"
#include "Datum.h"

#define PORT 8080

using namespace std;

piLed* piled = NULL;
bool running = true;

void signalHandler(int signum)
{
	//Detect if piled pointer has been set to an address
    if(piled){
		//Disable the LED
        piled -> disableLed();
		//Unexport the LED
        piled -> unexportPin();
    }
	//Set variable running to false
    running = false;
	//Print "exit command issued" to console
    cout << "exit command issued" << endl;
}

int main(int argc, char const *argv[])
{
	//Reroute ctrl+c to function signalHandler
    signal(SIGINT, signalHandler);

    //Create Logs
    ofstream bestand("/home/pi/Desktop/log.txt",  ios::out | ios::app);
    bestand << endl << "";

    //Create Commandline
    CommandLineInput comml("");

    //Create domotics objects
    Bed    bd(8080,      "bd", &comml, bestand);
    Column cl(8081, 550, "cl", &comml, bestand);
    Door   dr(8082, 2000,"dr", &comml, bestand);
    Chair  ch(8083, true,"ch", &comml, bestand);
	
	//Create pi LED
    piLed  led(0);
	
	//Link global pointer to variable led
    piled = &led;

    //Add relationships to objects
    cl.add(&led);
    dr.add(&bd);
    dr.add(&cl);

    while(running){
        //execute loop functions
        comml.loop();
        bd.behaviour();
        cl.behaviour();
        dr.behaviour();
        ch.behaviour();
    }
	//Write "afsluiten" in log
    bestand<<"afsluiten"<<endl;
	//Close log file
    bestand.close();
    return 0;
}
