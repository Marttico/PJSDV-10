

#include <iostream>
#include <string.h>
#include <thread>
#include <fstream>

#include "CommandLineInput.h"
#include "Chair.h"
#include "Door.h"
#include "Column.h"
#include "Bed.h"
#include "piLed.h"
#include "file.h"
#define PORT 8080

//std::fstream file;
//std::string locationOfLog;

using namespace std;


int main(int argc, char const *argv[])
{
    ofstream bestand("/home/pi/Desktop/log.txt",  ios::out | ios::app);
    bestand<<endl;
    bestand<<"";
    //File bestand("text.txt");
    //bestand<<"hello";
    //cout<<"hello"

    CommandLineInput comml("");
    Bed bd(8080,"bd",&comml, bestand);

    Column cl(8081,"cl",&comml, bestand);

    //Door dr(8082,10000,"dr",&comml, &cl, &bestand);

    piLed led(18);
    led.zetSpanningOpPin(false);
    int tf = 1;
    while(1){

        comml.loop();
        bd.behaviour();
        //cl.behaviour();
        //dr.behaviour();
        led.zetSpanningOpPin(tf);
        if(comml.getCLI()  == "stop")
        {
            break;
        }
    }
    bestand.close();

    return 0;
}
