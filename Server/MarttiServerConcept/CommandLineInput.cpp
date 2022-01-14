#include "CommandLineInput.h"

CommandLineInput::CommandLineInput(string i):clibuffer(i),executed(true){
    future = std::async(std::launch::async, getCommandLineInput);
}

CommandLineInput::~CommandLineInput(){

}

string* CommandLineInput::getCLIaddr(){
    return &clibuffer;
}

bool CommandLineInput::getExecuted(){
    return executed;
}
void CommandLineInput::setExecuted(){
    executed = true;
}
void CommandLineInput::clearCLI(){
    clibuffer[0] = 0;
}

string CommandLineInput::getCLI(){
    return clibuffer;
}
string CommandLineInput::getCommandLineInput(){
    string line;
    getline(std::cin,line);
    return line;
}

void CommandLineInput::loop(){
    //Check for new input


    //std::chrono::seconds timeout(5);
    //std::future<std::string> future = std::async(getCommandLineInput);
    //string newstring = "";
    //if(future.wait_for(timeout) == std::future_status::ready){
    //    newstring = future.get();
    //}
    
    //clibuffer += newstring;
    if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
        //Get function output
        clibuffer = future.get();
        //Turn executed flag to false
        executed = false;
        future = std::async(std::launch::async, getCommandLineInput);
        cout << clibuffer << endl;
    }

    //std::this_thread::sleep_for(std::chrono::milliseconds(5));
    
    
    
    //If last character of inputbuffer is \n then put inputbuffer into latestCommand string and clear inputbuffer
    //put input buffer into latestCommand
    //Clear inputbuffer
    //Turn executed flag to false


}