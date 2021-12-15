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

char ssid[] = "PiCo";     //  your network SSID (name)
char pass[] = "13371337";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
uint8_t ledValue = 0x00;

//WiFiServer wifiServer(8080);
const char* host = "192.168.2.1";
const int port = 8080;

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
    Wire.write(byte(input));
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
  Serial.begin(9600);
  while (!Serial);
  //WiFi.printDiag(Serial);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }
 
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
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
  static bool wait = false;
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);
  WiFiClient client;
  if (!client.connect(host, port)) {
      Serial.println("connection failed");
      delay(5000);
      return;
  }

  // This will send a string to the server
  Serial.println("sending data to server");
  if (client.connected()) {
      while(client.connected()){
        char sendBuf[6] = {'\0'};
        //Get Values I2C
        /*uint8_t InputOutput = readOutput();
        uint16_t Analog0 = readAnalog(0);
        uint16_t Analog1 = readAnalog(1);
        uint8_t Analog0_H = Analog0 >> 8;
        uint8_t Analog0_L = Analog0;
        uint8_t Analog1_H = Analog1 >> 8;
        uint8_t Analog1_L = Analog1;*/
        ledValue++;
        Serial.print("ledValue: ");
        Serial.println(ledValue);
        //Test Code without I2C
        uint8_t InputOutput = ledValue;
        uint8_t Analog0_H = 'a';
        uint8_t Analog0_L = 'b';
        uint8_t Analog1_H = 'c';
        uint8_t Analog1_L = 'd';
        
        //Convert all ints to chars
        char InputOutputc = int(InputOutput);
        char Analog0_Hc = int(Analog0_H);
        char Analog0_Lc = int(Analog0_L);
        char Analog1_Hc = int(Analog1_H);
        char Analog1_Lc = int(Analog1_L);
        
        //Append Values to Send Buffer
        strncat(sendBuf,&InputOutputc,1);
        strncat(sendBuf,&Analog0_Hc,1);
        strncat(sendBuf,&Analog0_Lc,1);
        strncat(sendBuf,&Analog1_Hc,1);
        strncat(sendBuf,&Analog1_Lc,1);
        Serial.println(sendBuf);
        //Send ProtocolVariables
        client.print(sendBuf);
        
        
        delay(100);
        
        //Maybe add an extra byte to set Servo Value
        char buf[2] = {'\0'};
        char ch;
        //Collect buffer to buf array
        while (client.available()) {
            ch = static_cast<char>(client.read());
            strncat(buf,&ch,1);
        }
        
        //Set I2C to first byte of Buffer.
        writeOutput(uint8_t(buf[0]));

        //Print Buffer to serial
        Serial.println(int(buf[0]));
        
        //Inside loop for debugging purpose (hot plugging wemos module into i/o board). 
        hotPluggableFunction();
      }
  }

  // wait for data to be available
  unsigned long timeout = millis();
  bool whileloop = true;
  while (client.available() == 0 && whileloop) {
      if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          delay(3000);
          //return;
          whileloop = false;
      }
  }
  
  Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
      char ch = static_cast<char>(client.read());
      Serial.print(ch);
  }

  //Close the connection
  Serial.println();
  Serial.println("closing connection");
  client.stop();

  if (wait) {
      delay(100); // execute once every 5 minutes, don't flood remote service
  }
  wait = true;
}
