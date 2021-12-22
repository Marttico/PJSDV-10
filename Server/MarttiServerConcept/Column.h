#ifndef COLUMN_H_
#define COLUMN_H_
#include "Wemos.h"
#include "CommandLineInput.h"
#include <thread>
#include <string>
//#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <functional>
#include <cstring>



using namespace std;
class Column
{
    public:
        Column(int , string , CommandLineInput* );
        ~Column();
        void zetZoemer(bool);
        void zetLed(bool);
        void behaviour();

    private:
        bool inputButton;
        bool gasSensor;
        bool ledMode;
        bool zoemerMode;
        //bool alarm;
        int port,sensorwaarde;
        Wemos wm;
        thread th;
        string prefix;
        CommandLineInput* cli;

        bool triggerCommands();
        void convertMessageToObjectAttr(char* );
        bool commandCompare(string);


};

#endif // ZUIL_H_
