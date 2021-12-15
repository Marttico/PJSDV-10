#include "file.h"
#include <cstring>


File::File(string location): locationOfLog(location)
{
    
}
void File::writeLog(string toWrite)
{
    file.open(locationOfLog, ios::app);
   

    time_t now = time(0);
    if(file.is_open())
    {

           char* dt = ctime(&now);
           if (dt[strlen(dt)-1] == '\n') 
                dt[strlen(dt)-1] = '\0';
           file <<  dt;

         //localTime->tm_year <<" <<": "<<toWrite<<endl;
         file<<":::"<< toWrite<<endl;
        file.close();
    }
    else
    {
        cout<<"Error writing to file "<< locationOfLog<<". \nSee line " <<__LINE__<< " of file "<<__FILE__ <<"."<<endl;\
        exit(-1);
    }

}

int File::getSizeOfLog() 
{
    file.open(locationOfLog, ios::in);
    cout << "op regel " << __LINE__ << "van bestand"<<__FILE__<<"is nog wat werk nodig"<<endl;
    
    file.close();
    return -1;
}
