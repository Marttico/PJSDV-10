#include "CommandLineInput.h"

CommandLineInput::CommandLineInput(string* CLIBuffer):clibuffer(CLIBuffer),th(&CommandLineInput::loop,this){

}

CommandLineInput::~CommandLineInput(){

}

void CommandLineInput::loop(){
    while(true){
        cin >> *clibuffer;
    }
}