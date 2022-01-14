#include "piLed.h"

piLed::piLed(int led):gpioNummer(led)
{
	
    exportPin();
}

piLed::~piLed()
{
    unexportPin();
}


int piLed::nummerGPIO() const{
	return gpioNummer;
}

void piLed::unexportPin()
{
	string path = "/sys/class/pwm/pwmchip0/unexport";

	ofstream fs;
	fs.open(path);
	fs << to_string(gpioNummer);
	fs.close();
}
void piLed::exportPin()
{
	string path = "/sys/class/pwm/pwmchip0/export";
	
	ofstream fs;
	fs.open(path);
	fs << to_string(gpioNummer);
	fs.close();
}

void piLed::zetPWM(int d,int p){
	
	string path = "/sys/class/pwm/pwmchip0/pwm" + to_string(gpioNummer) + "/period";
	ofstream fs;
	fs.open(path);
	fs << to_string(p);
	fs.close();	
	
	path = "/sys/class/pwm/pwmchip0/pwm" + to_string(gpioNummer) + "/duty_cycle";
	
	fs.open(path);
	fs << to_string(d);
	fs.close();	

	path = "/sys/class/pwm/pwmchip0/pwm" + to_string(gpioNummer) + "/enable";
	
	fs.open(path);
	fs << to_string(1);
	fs.close();	
}

void piLed::disableLed(){
	string path = "/sys/class/pwm/pwmchip0/pwm" + to_string(gpioNummer) + "/enable";
	ofstream fs;
	fs.open(path);
	fs << to_string(0);
	fs.close();	
}

void piLed::startFlashing(){
	if(!flashing){
		flashing = true;
	

		future = std::async(std::launch::async, [&](){
			int counter = 0;
			int output = 0;
			while(flashing || output > 50){
				output = cos((double)counter/10)*-10000+10000;
				//cout << flashing << endl;
				zetPWM(output,20000);
				usleep(1000000/60);
				counter++;
			}
		});
	}
	
}

void piLed::stopFlashing(){
	if(flashing){
		disableLed();
		flashing = false;
	}
}
string piLed::flashingFunc(){
	int counter = 0;
	while(flashing){
        zetPWM(cos((double)counter/10)*-10000+10000,20000);
        usleep(1000000/60);
		counter++;
    }
	return "";
}
