#include <iostream>
#include <string.h>
#include <thread>
#include "CommandLineInput.h"
#include "Chair.h"
#include "Column.h"
#include "Door.h"
#include "Bed.h"
#define PORT 8080
int main(int argc, char const *argv[])
{
    CommandLineInput comml("");
    Column cl(8082,"cl",&comml);
    Door dr(8081,2000,"dr",&comml);
    Bed bd(8080,"bd",&comml);
    Chair ch(8083,true,"ch", &comml);
    while(1){
        cl.behaviour();
        dr.behaviour();
        bd.behaviour();
        ch.behaviour();
        //printf("LOOP\n");
    }
    return 0;
}