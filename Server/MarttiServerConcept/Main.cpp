#include <iostream>
#include <string.h>
#include <thread>
#include "CommandLineInput.h"
#include "Chair.h"
#include "Door.h"
#include "Column.h"
#include "Bed.h"
#include "piLed.h"
#define PORT 8080
int main(int argc, char const *argv[])
{
    CommandLineInput comml("");
    Bed bd(8080,"bd",&comml);
    Column cl(8081,"cl",&comml);
    Door dr(8082,10000,"dr",&comml, &cl);

    piLed led(18);
    led.zetSpanningOpPin(true);
    int tf = 1;
    while(1){
        tf = tf ? 0 : 1;
        comml.loop();
        bd.behaviour();
        cl.behaviour();
        dr.behaviour();
        led.zetSpanningOpPin(tf);
    }
    return 0;
}
