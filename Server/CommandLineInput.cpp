#include "CommandLineInput.h"

CommandLineInput::CommandLineInput(string i):clibuffer(i),executed(true){
    //Turn on asynchronous function getCommandLineInput
    future = std::async(std::launch::async, getCommandLineInput);
}

CommandLineInput::~CommandLineInput(){

}

string* CommandLineInput::getCLIaddr(){
    //Return address of clibuffer
    return &clibuffer;
}

bool CommandLineInput::getExecuted(){
    //Return whether the latest command has been executed
    return executed;
}
void CommandLineInput::setExecuted(){
    //Set private variable executed to true
    executed = true;
}
void CommandLineInput::clearCLI(){
    //Clear clibuffer
    clibuffer[0] = 0;
}

string CommandLineInput::getCLI(){
    //Get latest command
    return clibuffer;
}

//Get string from console
string CommandLineInput::getCommandLineInput(){
    string line;
    getline(std::cin,line);
    return line;
}

void CommandLineInput::loop(){
    //Check for new input
    if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        //Get function output
        clibuffer = future.get();
        //Turn executed flag to false
        executed = false;
        //Restart async thread
        future = std::async(std::launch::async, getCommandLineInput);
        //Print latest command to console
        cout << clibuffer << endl;
    }
}