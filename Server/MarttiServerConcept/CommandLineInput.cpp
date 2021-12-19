#include "CommandLineInput.h"

CommandLineInput::CommandLineInput(string* CLIBuffer):clibuffer(CLIBuffer),th(&CommandLineInput::loop,this){

}
CommandLineInput::~CommandLineInput(){

}
void CommandLineInput::loop(){
    while(1){
        cin >> *clibuffer;
    }
}