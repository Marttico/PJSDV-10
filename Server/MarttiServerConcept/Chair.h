#include "Wemos.h"
#ifndef CHAIR_H_
#define CHAIR_H_

#include <thread>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <functional>

using namespace std;

class Chair {
public:
    Chair(int,bool,string,string*);
    ~Chair();
    void zetTril(bool);
    void zetTrilPermissie(bool);
    void zetLed(bool);
    void behaviour();
    void convertMessageToObjectAttr(char*);
    bool triggerCommands();
private:
    bool trilMode;
    bool trilPerms;
    bool ledMode;
    bool inputButton;
    uint16_t inputPressure;
    int port;
    Wemos wm;
    thread th;
    string prefix;
    string* commandLine;
    void commandCompare(string, void (Chair::*)(bool), bool, bool*);
};

#endif
