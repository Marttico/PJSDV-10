#include <iostream>
#include <string.h>
#include <thread>
#include <math.h>
#include "CommandLineInput.h"
#include "Chair.h"
#include "Door.h"
#include "Column.h"
#include "Bed.h"
#include "piLed.h"
#include "file.h"

#define PORT 8080
int main(int argc, char const *argv[])
{
    File f("test.txt");
    f.writeLog("Server gestart!");
    CommandLineInput comml("");
    Bed bd(8080,"bd",&comml, &f);
    Column cl(8081,"cl",&comml, &f);
    Door dr(8082,2000,"dr",&comml);
    piLed led(0);
    cl.add(&led);
    dr.add(&bd);
    dr.add(&cl);
    
    int tf = 1;
    //led.disableLed();

    while(1){

        comml.loop();
        bd.behaviour();
        cl.behaviour();
        dr.behaviour();

        /*if(bd.InputPressure() > 600){
            cout << "startflashing" << endl;
            led.startFlashing();
        }else{
            cout << "stopflashing" << endl;
            led.stopFlashing();
        }*/
    }
    return 0;
}
