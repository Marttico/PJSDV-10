#include "Wemos.h"
#include "CommandLineInput.h"

#ifndef CHAIR_H_
#define CHAIR_H_

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

class Chair {
public:
    Chair(int, bool, string, CommandLineInput*);
    ~Chair();
    void zetTril(bool), zetTrilPermissie(bool), zetLed(bool);
    void behaviour();
private:
    bool trilMode, trilPerms, ledMode, inputButton;
    int port;
    uint16_t inputPressure;
    string prefix;
    Wemos wm;
    CommandLineInput* cli;
    bool triggerCommands(), commandCompare(string);
    void convertMessageToObjectAttr(char*);
};

#endif
