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
#include<string.h>
#include<stdio.h>
#include <stdlib.h>

#define I2C_SDL    D1
#define I2C_SDA    D2

char ssid[] = "PiCo";     //  your network SSID (name)
char pass[] = "13371337";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
uint8_t ledValue = 0x00;

//WiFiServer wifiServer(8080);
const char* host = "192.168.2.1";
const int port = 8080;

WiFiClient client;


void setup() {
  pinMode(D5, OUTPUT);
  Wire.begin();
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  writeOutput(255);
  delay(1000);
  writeOutput(0);
  
  
  
  Serial.begin(115200);
  while (!Serial);
  //WiFi.printDiag(Serial);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }
 
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Connecting...");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // wait 6 seconds for connection:
    delay(10000);
  }
  Serial.print("You're connected to the network");
}

// the loop function runs over and over again forever
void loop() {
  hotPluggableFunction();
  if (!client.connect(host, port)){
    Serial.println("Connection to host failed");
    delay(1000);
    return;
  }
  Serial.println("Connected to server successful!");
  while(client.connected()){
    char buf[1024] = {0};
    int index = 0;
    char c = 0;
    while (client.available() > 0 && c != '\r')
    {
      c = client.read();
      buf[index] = c;
      index++;
    }
    buf[index] = '\0';
    
    if(buf[0] == 'R'){
      Serial.println(buf);
      //Serial.println("Read Command Received");
      char sendBuffer[1024] = {0};
      sprintf(sendBuffer,"%i,%i,%i",readOutput(),readAnalog(0),readAnalog(1));
      client.print(sendBuffer);
    }
    if(buf[0] == 'W'){
      Serial.println(buf);
      //Serial.println("Write Command Received");
      char *ptr;
      char *p;
      p = strtok(buf,",");
      //Serial.println(p);
      p = strtok(NULL,",");
      //Serial.println(p);
      uint8_t first = strtol(p,&ptr,10);
      writeOutput(first);
      p = strtok(NULL,",");
      //Serial.println(p);
      uint16_t second = strtol(p,&ptr,10);
    }
    
  }
  //Serial.print('\n');
  client.stop();
  delay(5000);
}


void writeOutput(uint8_t input){
    Wire.beginTransmission(0x38); 
    Wire.write(byte(0x01));
    Wire.write(byte(input&0xF0));
    Wire.endTransmission();
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
