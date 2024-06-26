#include "Wemos.h"
#include "CommandLineInput.h"
#include "Datum.h"
#ifndef BED_H_
#define BED_H_

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

class Bed {
public:
    Bed(int, string, CommandLineInput*, ofstream&);
    ~Bed();
    void zetLed(bool);
    void behaviour();
    int InputPressure() const;
    bool hankIsOnBed;
private:
    bool ledMode, inputButton,oldInputButton;
    uint16_t inputPressure;
    uint64_t bedTimer, getMillis();
    
    int port;
    string prefix;
    Wemos wm;
    ofstream& bestand;
    Datum ddd;
    CommandLineInput* cli;
    bool commandCompare(string);
    void triggerCommands(),convertMessageToObjectAttr(char*);
};

#endif
