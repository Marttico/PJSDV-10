#include "Wemos.h"
#include "CommandLineInput.h"

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
    Bed(int, string, CommandLineInput*);
    ~Bed();
    void zetLed(bool);
    void behaviour();
private:
    bool ledMode, inputButton;
    uint16_t inputPressure;
    uint64_t bedTimer, getMillis();
    int port;
    string prefix;
    Wemos wm;
    CommandLineInput* cli;
    bool triggerCommands(), commandCompare(string);
    void convertMessageToObjectAttr(char*);
};

#endif
