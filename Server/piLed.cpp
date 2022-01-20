#include "piLed.h"
//Initialise object with flashingCheckThread alongside it on another thread
piLed::piLed(int led):gpioNummer(led),th(&piLed::flashingCheckThread,this)
{
	//Export PWM pin when object gets created
    exportPin();
}

piLed::~piLed()
{
	//unexport PWM pin when object gets deleted
    unexportPin();
}


int piLed::nummerGPIO() const{
	return gpioNummer;
}

void piLed::unexportPin()
{
	//Write the gpioNummer variable to the file unexport in the PWMchip0 folder of the Raspberry Pi
	string path = "/sys/class/pwm/pwmchip0/unexport";

	ofstream fs;
	fs.open(path);
	fs << to_string(gpioNummer);
	fs.close();
}

void piLed::exportPin()
{
	//Write the gpioNummer variable to the file export in the PWMchip0 folder of the Raspberry Pi
	string path = "/sys/class/pwm/pwmchip0/export";
	
	ofstream fs;
	fs.open(path);
	fs << to_string(gpioNummer);
	fs.close();
}

void piLed::zetPWM(int d,int p){
	
	//Write the period of the PWM timer to the PWM pin
	string path = "/sys/class/pwm/pwmchip0/pwm" + to_string(gpioNummer) + "/period";
	ofstream fs;
	fs.open(path);
	fs << to_string(p);
	fs.close();	

	//Write the duty cycle of the PWM timer to the PWM pin
	path = "/sys/class/pwm/pwmchip0/pwm" + to_string(gpioNummer) + "/duty_cycle";
	fs.open(path);
	fs << to_string(d);
	fs.close();	

	//Write 1 to the PWM pin enable file (this is to enable the actual pin)
	path = "/sys/class/pwm/pwmchip0/pwm" + to_string(gpioNummer) + "/enable";
	fs.open(path);
	fs << to_string(1);
	fs.close();	
}

void piLed::disableLed(){
	//Write 0 to the PWM pin enable file (this is to enable the actual pin)
	string path = "/sys/class/pwm/pwmchip0/pwm" + to_string(gpioNummer) + "/enable";
	ofstream fs;
	fs.open(path);
	fs << to_string(0);
	fs.close();	
}

//Set variable flashing to true once
void piLed::startFlashing(){
	if(!flashing){
		flashing = true;
	}
}

//Set variable flashing to false once
void piLed::stopFlashing(){
	if(flashing){
		disableLed();
		flashing = false;
	}
}

void piLed::flashingCheckThread(){
	while(1){
		int counter = 0;
		int output = 0;
		//Check whether the lamp is supposed to be flashing or whether the output is higher than 50
		while(flashing || output > 50){
			//This formula calculates the duty cycle of the PWM signal
			output = cos((double)counter/10)*-10000+10000;
			//Set duty cycle to the output variable
			zetPWM(output,20000);
			//Wait 1/60th of a second (or 1/60th of 1000000 microseconds)
			usleep(1000000/60);
			counter++;
		}
	}
}