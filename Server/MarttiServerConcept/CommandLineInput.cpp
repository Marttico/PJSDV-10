#include "CommandLineInput.h"

CommandLineInput::CommandLineInput(string i):clibuffer(i),executed(true){

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
    auto future = std::async(std::launch::async, getCommandLineInput);

    while (true) {
        if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto line = future.get();

            // Set a new line. Subtle race condition between the previous line
            // and this. Some lines could be missed. To aleviate, you need an
            // io-only thread. I'll give an example of that as well.
            future = std::async(std::launch::async, getCommandLineInput);

            std::cout << "you wrote " << line << std::endl;
        }

        std::cout << "waiting..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    
    
    //If last character of inputbuffer is \n then put inputbuffer into latestCommand string and clear inputbuffer
    //put input buffer into latestCommand
    //Clear inputbuffer
    //Turn executed flag to false


}