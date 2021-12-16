 /*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain
  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)
  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/
#include <ESP8266WiFi.h>
#include <Wire.h>

#define I2C_SDL    D1
#define I2C_SDA    D2

const char* ssid = "PiCo";     //  your network SSID (name)
const char* password = "13371337";  // your network password

WiFiServer wifiServer(8080);

int status = WL_IDLE_STATUS;     // the Wifi radio's status
uint8_t ledValue = 0x00;

void writeOutput(bool b,int offset){
    //offset = 1 for the lil light :)
    Wire.beginTransmission(0x38); 
    Wire.write(byte(0x00));      
    Wire.endTransmission();
    Wire.requestFrom(0x38, 1);   
    uint8_t inputs = Wire.read(); 
    if(b){
      inputs |= 1<<offset+4;
    }else{
      inputs &= ~(1<<offset+4);
    }
    Wire.beginTransmission(0x38); 
    Wire.write(byte(0x01));           
    Wire.write(byte(inputs));
    Wire.endTransmission();
}
void writeOutput(uint8_t input){
    Wire.beginTransmission(0x38); 
    Wire.write(byte(0x01));
    Wire.write(byte(input &0xF0));
    Wire.endTransmission();
}

uint8_t readOutput(int outputSlot){
    Wire.beginTransmission(0x38); 
    Wire.write(byte(0x00));      
    Wire.endTransmission();
    Wire.requestFrom(0x38, 1);   
    uint8_t inputs = Wire.read();  
    //return (inputs && (1 << outputSlot)) >> outputSlot;
    return (inputs >> outputSlot) & 1;
    //return inputs & 0x01;
}
uint8_t readOutput(){
    Wire.beginTransmission(0x38); 
    Wire.write(byte(0x00));      
    Wire.endTransmission();
    Wire.requestFrom(0x38, 1);   
    uint8_t inputs = Wire.read();
    return inputs;
}

uint16_t readAnalog(int slot){
    //Read analog 10bit inputs 0&1
    Wire.requestFrom(0x36, 4);   
    uint16_t anin0 = Wire.read()&0x03;  
    anin0=anin0<<8;
    anin0 = anin0|Wire.read();  
    uint16_t anin1 = Wire.read()&0x03;  
    anin1=anin1<<8;
    anin1 = anin1|Wire.read(); 

    if(slot == 0){
      return anin0;
    }else if(slot == 1){
      return anin1;
    }
}
void hotPluggableFunction(){
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x03));          
    Wire.write(byte(0x0F));         
    Wire.endTransmission();
}

void setup() {
    pinMode(D5, OUTPUT);
    Wire.begin();
    pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
    Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
    writeOutput(0b00110101);
    Serial.write(0b00110101);

  
    
    delay(100);
}
