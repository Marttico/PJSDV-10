#include <iostream>
#include <string.h>
#include <thread>
#include "CommandLineInput.h"
#include "Chair.h"
#include "Door.h"
#define PORT 8080
int main(int argc, char const *argv[])
{
    CommandLineInput comml("");
    //Chair ch1(8080,true,"ch",comml.getCLIaddr());
    Door dr(8080,"dr",comml.getCLIaddr());
    while(1){
        
    }
    return 0;
}