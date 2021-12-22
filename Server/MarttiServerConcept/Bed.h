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
    Bed(int,string,CommandLineInput*);
    ~Bed();
    void zetLed(bool);
    void behaviour();

private:
    uint64_t bedTimer;
    bool ledMode;
    bool inputButton;
    uint16_t inputPressure;
    int port;
    Wemos wm;
    thread th;
    string prefix;
    CommandLineInput* cli;
    bool triggerCommands();
    void convertMessageToObjectAttr(char*);
    bool commandCompare(string);
    uint64_t getMillis();
};

#endif
