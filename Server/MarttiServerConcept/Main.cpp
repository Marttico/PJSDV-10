#include <iostream>
#include <string.h>
#include <thread>
#include "CommandLineInput.h"
#include "Chair.h"
#define PORT 8080
int main(int argc, char const *argv[])
{
    string cli;
    CommandLineInput clinput(&cli);
    Chair ch1(8080,true,"ch",&cli);
    while(1){
        
    }
    return 0;
}