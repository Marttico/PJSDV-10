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
    Chair ch(8080,true,"ch",&cli);
    //Chair ch2(8081,true);
    
    while(1){
        
    }
    return 0;
}