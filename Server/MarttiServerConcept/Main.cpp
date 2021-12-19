#include <iostream>
#include <string.h>
#include "Chair.h"
#define PORT 8080
int main(int argc, char const *argv[])
{
    Chair ch(8080,true);
    
    while(1){
        ch.behaviour();
    }
    return 0;
}