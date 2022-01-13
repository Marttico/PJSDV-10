
#ifndef FILE_H
#define FILE_H
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
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
#endif
