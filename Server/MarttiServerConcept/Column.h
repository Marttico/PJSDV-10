#include "Wemos.h"
#include "CommandLineInput.h"
#include "file.h"
#include "piLed.h"
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
        Column(int, string, CommandLineInput*, File* );
        ~Column();
        void zetZoemer(bool), zetLed(bool), printValue();
        int isBrand() const;
        void behaviour(), add(piLed*);

    private:
        bool inputButton,gasSensor,ledMode,zoemerMode;
        int port,sensorwaarde;
        int brand =false;

        string prefix;
        Wemos wm;
        piLed* pl;
        File* fi;
        CommandLineInput* cli;
        bool commandCompare(string);
        void triggerCommands(), convertMessageToObjectAttr(char*);
};

#endif // ZUIL_H_
