#include "Wemos.h"
#include "CommandLineInput.h"

#ifndef COLUMN_H_
#define COLUMN_H_

#include <chrono>
#include <thread>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <sys/time.h>
#include <iostream>
#include <ctime>

using namespace std;

class Column {
    public:
        Column(int, string, CommandLineInput* );
        ~Column();
        void zetZoemer(bool), zetLed(bool);
        int sensorWaarde() const;
        void behaviour();
        int brand =false;
    private:
        bool inputButton,gasSensor,ledMode,zoemerMode;
        int port,sensorwaarde;

        string prefix;
        Wemos wm;
        CommandLineInput* cli;
        bool commandCompare(string);
        void triggerCommands(), convertMessageToObjectAttr(char*);
};

#endif // ZUIL_H_
