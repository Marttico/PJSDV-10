#include "Datum.h"



Datum::Datum(){}

//Return current date and time
std::ostream& operator<<(std::ostream& s, const Datum& ds)
{
    time_t now = time(0); //haal huidige tijd op
    char* dt = ctime(&now); //zet tijd om in standaart leesbaar formaat in char*
    if (dt[strlen(dt)-1] == '\n')
    dt[strlen(dt)-1] = '\0'; //haal enter weg op de laatste regel: staat niet netjes in logboek

    s<<dt<<":::"; //voeg datumstring toe aan de huidige string met dubbele punten voor leesbaarheid
    return s; //return voor overloading
}
