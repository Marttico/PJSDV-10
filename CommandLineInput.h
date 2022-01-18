#include <string>
#include <string.h>
#include <iostream>
#include <thread>
#include <future>
#include <chrono>

#ifndef COMMANDLINEINPUT_H_
#define COMMANDLINEINPUT_H_

using namespace std;


class CommandLineInput {
public:
    CommandLineInput(string);
    ~CommandLineInput();
    void loop();
    string* getCLIaddr();
    void clearCLI();
    string getCLI();
    bool getExecuted();
    void setExecuted();
private:
    static string getCommandLineInput();
    bool executed;
    string clibuffer;
    std::future<std::string> future;
    
};

#endif
