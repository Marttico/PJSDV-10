#include "Wemos.h"
#include "CommandLineInput.h"
#include "piLed.h"
#include "Datum.h"
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
        Column(int, int, string, CommandLineInput*, ofstream&);
        ~Column();
        void zetZoemer(bool), zetLed(bool), printValue();
        int isBrand() const;
        void behaviour(), add(piLed*);

    private:
        bool inputButton,gasSensor,ledMode,zoemerMode;
        int port,sensorwaarde;
        int brand;
        int brandwaarde;
        int brandTemp;
        string prefix;
        Wemos wm;
        piLed* pl;
        ofstream& bestand;
        Datum ddd;
        CommandLineInput* cli;
        bool commandCompare(string);
        void triggerCommands(), convertMessageToObjectAttr(char*);
};

#endif // ZUIL_H_
