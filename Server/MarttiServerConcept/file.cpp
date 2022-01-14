#include "file.h"



Datum::Datum()//: locationOfLog(location)
{
        //  locationOfLog(location);
}
/*int File:dummy(int a) const
{
    return a;
}
File& operator<<(File& FSS, const char& s)
{

    //File tmp(*this);
    tmp.writeLog(s);
    //tmp+=FSS;
    return FSS;
}*/
std::ostream& operator<<(std::ostream& s, const Datum& ds)
{
    //file.open(locationOfLog, ios::app);
    //test_main++;

    time_t now = time(0);
    char* dt = ctime(&now);
    if (dt[strlen(dt)-1] == '\n')
    dt[strlen(dt)-1] = '\0';

    s<<dt<<":::";
    /*if(file.is_open())
    {


           file <<  dt;

         //localTime->tm_year <<" <<": "<<toWrite<<endl;
         file<<":::"<< toWrite<<endl;
        file.close();
    }
    else
    {
       // cout<<"Error writing to file "<< locationOfLog<<". \nSee line " <<__LINE__<< " of file "<<__FILE__ <<"."<<endl;\
        //exit(-1);
    }
*/
}

