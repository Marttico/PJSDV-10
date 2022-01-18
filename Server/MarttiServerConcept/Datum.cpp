#include "Datum.h"



Datum::Datum()//: locationOfLog(location)
{
        //  locationOfLog(location);
}

std::ostream& operator<<(std::ostream& s, const Datum& ds)
{
    time_t now = time(0);
    char* dt = ctime(&now);
    if (dt[strlen(dt)-1] == '\n')
    dt[strlen(dt)-1] = '\0';

    s<<dt<<":::";
    return s;

}