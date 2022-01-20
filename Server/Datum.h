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
    Datum();
    friend ostream& operator<<(ostream&, const Datum& );
};
#endif