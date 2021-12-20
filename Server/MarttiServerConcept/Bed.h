#include "Wemos.h"
#ifndef BED_H_
#define BED_H_

#include <thread>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <functional>

using namespace std;

class Bed {
public:
    Bed(int,string,string*);
    ~Bed();
    void zetLed(bool);
    void behaviour();
private:
    bool ledMode;
    bool inputButton;
    uint16_t inputPressure;
    int port;
    Wemos wm;
    thread th;
    string prefix;
    string* commandLine;
    bool triggerCommands();
    void convertMessageToObjectAttr(char*);
    void commandCompare(string, void (Bed::*)(bool), bool, bool*);
};

#endif
