#include "Wemos.h"
#include "CommandLineInput.h"

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
    Door(int, int, string, CommandLineInput*);
    ~Door();
    void zetLed(bool), zetDoorAngle(int), zetDebugButton(bool), zetOpenPermissie(bool);
    void behaviour();
private:
    bool ledMode,inputButton,oldInputButton,openPermissie;
    int doorAngle,port,doorOpenTimerDelay;
    uint64_t doortimer,ledtimer, getMillis();
    string prefix;
    Wemos wm;
    CommandLineInput* cli;
    bool triggerCommands(), commandCompare(string);
    void convertMessageToObjectAttr(char*);
};

#endif
