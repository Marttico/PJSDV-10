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
    Bed bd(8080,"bd",&comml);
    while(1){
        comml.loop();
        bd.behaviour();
        cout << "test" << endl;
    }
    return 0;
}
