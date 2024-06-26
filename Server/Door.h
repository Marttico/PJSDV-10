#include "Wemos.h"
#include "CommandLineInput.h"
#include "Column.h"
#include "Bed.h"
#include "Datum.h"

#ifndef DOOR_H_
#define DOOR_H_

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

class Door {
public:
    Door(int, int, string, CommandLineInput*, ofstream&);
    ~Door();
    void add(Column*),add(Bed*), zetLed(bool), zetDoorAngle(int), zetDebugButton(bool), zetOpenPermissie(bool);
    void behaviour();
private:
    bool ledMode,inputButton,oldInputButton,openPermissie;
    int doorAngle,port,doorOpenTimerDelay;
    uint64_t doortimer,ledtimer, getMillis();
    bool ledLogTimer;
    string prefix;
    Wemos wm;
    ofstream& bestand;
    Datum ddd;
    CommandLineInput* cli;
    Bed* bed;
    bool commandCompare(string);
    void triggerCommands(),convertMessageToObjectAttr(char*);
    
    Column* cl;
};

#endif
