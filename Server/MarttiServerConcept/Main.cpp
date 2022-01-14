#include <iostream>
#include <string.h>
#include <thread>
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

    dr.add(&bd);
    dr.add(&cl);
    piLed led(18);
    led.zetSpanningOpPin(false);
    int tf = 1;
    while(1){

        comml.loop();
        bd.behaviour();
        cl.behaviour();
        dr.behaviour();
        led.zetSpanningOpPin(tf);
    }
    return 0;
}
