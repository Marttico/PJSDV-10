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

bool running = true;

void signalHandler(int signum)
{
  running = false;
  cout << "exit command issued" << endl;
}

int main(int argc, char const *argv[])
{
    cout << "  ____  _____            _   _ _____  " << endl << " |  _ \\|  __ \\     /\\   | \\ | |  __ \\ " << endl << " | |_) | |__) |   /  \\  |  \\| | |  | |" << endl << " |  _ <|  _  /   / /\\ \\ | . ` | |  | |" << endl << " | |_) | | \\ \\  / ____ \\| |\\  | |__| |" << endl <<" |____/|_|  \\_\\/_/    \\_\\_| \\_|_____/ " << endl;
    signal(SIGINT, signalHandler); //Voor ctrl + c

    //Create Logs
    ofstream bestand("/home/pi/Desktop/log.txt",  ios::out | ios::app);
    bestand << endl << "";
    //Create Commandline
    CommandLineInput comml("");

    //Create domotics objects
    Bed    bd(8080,      "bd", &comml, bestand);
    Column cl(8081, 400, "cl", &comml, bestand);
    Door   dr(8082, 2000,"dr", &comml, bestand);
    Chair  ch(8083, true,"ch", &comml, bestand);
    piLed  led(0);


    //Add relationships to objects
    cl.add(&led);
    dr.add(&bd);
    dr.add(&cl);

    while(running){
        comml.loop();
        bd.behaviour();
        cl.behaviour();
        dr.behaviour();
    }
    bestand.close();
    return 0;
}
