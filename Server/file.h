#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream> 
#include <ctime>
using namespace std;
class File
{
    public:
        File(string );
        void writeLog(string);
        int getSizeOfLog() ; 
    private:
        string locationOfLog;
        fstream file;  
};
