#ifndef ZUIL_H_
#define ZUIL_H_
#include "Wemos.h"

#include <thread>
#include <string>
//#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <functional>
#include <cstring>

using namespace std;
class Zuil
{
    public:
        Zuil(int , string , string* );
        ~Zuil();
        void zetZoemer(bool);
        void zetLed(bool);
        void behaviour();

    private:
        bool inputButton;
        bool gasSensor;
        bool ledMode;
        bool zoemerMode;
        //bool alarm;
        int port;
        Wemos wm;
        thread th;
        string prefix;
        string* commandLine;

        bool triggerCommands();
        void convertMessageToObjectAttr(char* );
        void commandCompare(string, void(Zuil::*)(bool), bool, bool*);


};

#endif // ZUIL_H_
