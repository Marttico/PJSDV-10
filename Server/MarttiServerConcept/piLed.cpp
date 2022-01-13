#include "piLed.h"

piLed::piLed(int led):gpioNummer(led)
{
    //ctor
    string s=to_string(gpioNummer);
	zetPin("export",s);
	usleep(250000); // 250ms delay
	zetPin("direction","out");

}

piLed::~piLed()
{
    //dtor\cout<<"platform verwijderd"<<gpioNummer<<endl;
	string s=to_string(gpioNummer);
	zetPin("unexport",s);
}
int piLed::nummerGPIO() const{
	return gpioNummer;
}
void piLed::zetSpanningOpPin(bool b)
{
	//cout << "Spanning op pin: "<< b<<" pin: "<<gpioNummer<<endl;
   if(b)
	   zetPin("value","1");
   else
	   zetPin("value","0");
}

void piLed::ontkoppel()
{
	string s=to_string(gpioNummer);
	zetPin("unexport",s);
}
void piLed::zetPin(string s,string value)
{

	stringstream p;
	p << GPIO_PATH;
	if(s != "export" && s != "unexport")
		p <<"gpio"<< gpioNummer<<"/";

	p<<s;
	string naam=p.str();
	ofstream fs;

	fs.open(naam);
	fs << value;
	fs.close();
}
