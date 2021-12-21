#include <iostream>
#include <string.h>
#include <thread>
#include "CommandLineInput.h"
#include "Chair.h"
#include "Column.h"
#include "Door.h"
#define PORT 8080
int main(int argc, char const *argv[])
{
    CommandLineInput comml("");
    Column cl(8082,"cl",comml.getCLIaddr());
    Door dr(8081,2000,"dr",comml.getCLIaddr());
    Chair ch(8080,true,"ch",comml.getCLIaddr());
    
    while(1){
        
    }
    return 0;
}