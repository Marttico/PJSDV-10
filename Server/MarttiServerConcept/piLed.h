#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <future>
#include <chrono>
#include <unistd.h>
#include <math.h>

#define GPIO_PATH "/sys/class/gpio/"
#ifndef PILED_H
#define PILED_H

using namespace std;
class piLed
{
    public:
        piLed(int);
        virtual ~piLed();
        void zetPWM(int,int);
        int nummerGPIO() const;
        void exportPin();
        void unexportPin();
        void disableLed();
        void startFlashing();
        void stopFlashing();
        void flashingCheckThread();
    private:
        int gpioNummer;
        thread th;
        bool flashing;
        std::future<void> future;
};

#endif // PILED_H
