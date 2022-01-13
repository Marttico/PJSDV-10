#ifndef PILED_H
#define PILED_H

#define GPIO_PATH "/sys/class/gpio/"

#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
using namespace std;
class piLed
{
    public:
        piLed(int);
        virtual ~piLed();

        int nummerGPIO() const;
        void zetSpanningOpPin(bool);
        void ontkoppel();
        void zetPin(string, string);
    private:
        int gpioNummer;
};

#endif // PILED_H
