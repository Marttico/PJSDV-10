#include "Wemos.h"
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
    Door(int,string,string*);
    ~Door();
    void zetLed(bool);
    void zetDoorAngle(int);
    void zetDebugButton(bool);
    void behaviour();
private:
    bool ledMode;
    int doorAngle;
    bool inputButton;
    bool oldInputButton;
    uint64_t doortimer;
    int port;
    Wemos wm;
    thread th;
    string prefix;
    string* commandLine;
    bool triggerCommands();
    void convertMessageToObjectAttr(char*);
    void commandCompare(string, void (Door::*)(bool), bool, bool*);
    void commandCompare(string, void (Door::*)(int), int, bool*);
    uint64_t getMillis();
};

#endif
