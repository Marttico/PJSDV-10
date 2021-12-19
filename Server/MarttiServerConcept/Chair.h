#include "Wemos.h"
#ifndef CHAIR_H_
#define CHAIR_H_

#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Chair {
public:
    Chair(int,bool);
    ~Chair();
    void zetTril(bool);
    void zetTrilPermissie(bool);
    void zetLed(bool);
    void behaviour();
private:
    bool trilMode;
    bool trilPerms;
    bool ledMode;
    bool inputButton;
    uint16_t inputPressure;
    int port;
    Wemos wm;
};

#endif
