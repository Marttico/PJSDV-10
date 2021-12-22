#include "CommandLineInput.h"

CommandLineInput::CommandLineInput(string i):clibuffer(i),isBusy(false),th(&CommandLineInput::loop,this){

}

CommandLineInput::~CommandLineInput(){

}

string* CommandLineInput::getCLIaddr(){
    return &clibuffer;
}

bool CommandLineInput::checkBusy(){
    return isBusy;
}
void CommandLineInput::setBusy(bool i){
    isBusy = i;
}
void CommandLineInput::clearCLI(){
    clibuffer[0] = 0;
}

string CommandLineInput::getCLI(){
    return clibuffer;
}
void CommandLineInput::loop(){
    while(true){
        cin >> clibuffer;
    }
}