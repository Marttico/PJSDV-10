#include <string>
#include <string.h>
#include <iostream>
#include <thread>
#ifndef COMMANDLINEINPUT_H_
#define COMMANDLINEINPUT_H_

using namespace std;


class CommandLineInput {
public:
    CommandLineInput(string);
    ~CommandLineInput();
    void loop();
    string* getCLIaddr();
    bool checkBusy();
    void setBusy(bool);
    void clearCLI();
    string getCLI();
private:
    bool isBusy;
    string clibuffer;
    thread th;
};

#endif
