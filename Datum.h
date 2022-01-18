#ifndef DATUM_H
#define DATUM_H
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>


using namespace std;

class Datum
{
public:
    Datum( );
    //void writeLog(string);
    //int getSizeOfLog() ;
    //static int dum= 1;
    friend ostream& operator<<(ostream&, const Datum& );


private:
   // fstream file;
    //string locationOfLog;
};
#endif