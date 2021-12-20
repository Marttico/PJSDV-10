#include "CommandLineInput.h"

CommandLineInput::CommandLineInput(string i):clibuffer(i),th(&CommandLineInput::loop,this){

}

CommandLineInput::~CommandLineInput(){

}

string* CommandLineInput::getCLIaddr(){
    return &clibuffer;
}

void CommandLineInput::loop(){
    while(true){
        cin >> clibuffer;
    }
}