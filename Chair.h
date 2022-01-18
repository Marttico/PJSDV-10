#include "Wemos.h"
#include "CommandLineInput.h"
#include "Datum.h"

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
    Chair(int, bool, string, CommandLineInput*, ofstream&);
    ~Chair();
    void zetTril(bool), zetTrilPermissie(bool), zetLed(bool);
    void behaviour();
private:
    bool trilMode, trilPerms, ledMode, inputButton;
    int port;
    int tril;
    bool hankIsOnChair;
    uint16_t inputPressure;
    string prefix;
    Wemos wm;
    ofstream& bestand;
    Datum ddd;
    CommandLineInput* cli;
    bool triggerCommands(), commandCompare(string);
    void convertMessageToObjectAttr(char*);
};

#endif
