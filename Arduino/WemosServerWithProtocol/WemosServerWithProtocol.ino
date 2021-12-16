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
    delay(1000);
   
    WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
 
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());
 
  wifiServer.begin();
}

// the loop function runs over and over again forever
void loop() {
  WiFiClient client = wifiServer.available();
  hotPluggableFunction();
  if (client) {
    while(client.connected()){
      char buffer[64] = {0};
      int index = 0;
      while (client.available()>0) {
        char c = client.read();
        //strncat(buffer,&c,1);
        buffer[index] = c;
        index++;
      }
      buffer[index] = '\0';
      if(buffer[0] == 1){
        //Read Frame
        char InputOutput = readOutput();
        //uint16_t Analog0 = readAnalog(0);
        //uint16_t Analog1 = readAnalog(1);
        uint16_t Analog0 = 0;
        uint16_t Analog1 = 0;
        char Analog0_H = Analog0 >> 8;
        char Analog0_L = Analog0;
        char Analog1_H = Analog1 >> 8;
        char Analog1_L = Analog1;



        
        //Test Code without I2C
        /*Serial.print("ledValue: ");
        Serial.println(ledValue);
        char InputOutput = ledValue;
        char Analog0_H = 'a';
        char Analog0_L = 'b';
        char Analog1_H = 'c';
        char Analog1_L = 'd';*/
        //Append Values to Send Buffer
        char msg[] = {InputOutput,Analog0_H,Analog0_L,Analog1_H,Analog1_L,'\r','\0'};
        client.write(msg);
        //Serial.println(sendBuf);


        
      }else if(buffer[0] == 2){
        Serial.println();
        writeOutput(buffer[1]);
      }
      //delay(100);
    }
    
  }
  client.stop();
  //Serial.println("Client Disconnected");
}
