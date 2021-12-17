#include <ESP8266WiFi.h>
#include <Wire.h>


#define I2C_SDL    D1
#define I2C_SDA    D2

const char* ssid = "PiCo";     //  your network SSID (name)
const char* password = "13371337";  // your network password

WiFiServer wifiServer(8080);


void setup(void) {
  pinMode(D5, OUTPUT);
  Wire.begin();
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
 
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());
 
  wifiServer.begin();
  wifiServer.setNoDelay(true);
}
 int delayval = 500; // delay for half a second
 // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
 

unsigned int outputs=0;
void loop(void) {
  WiFiClient client = wifiServer.available();
  client.setNoDelay(true);
  hotplug();
  if(client){
    while(client.connected()){
      char buf[64] = {0};
      while(client.available() > 0){
        char c = client.read();
        strncat(buf,&c,1);
      }
      client.flush();
      
      if(buf[0] == 0x01){
        Serial.println("Read");
        //Get Values I2C
        char InputOutput = readOutput();
        uint16_t Analog0 = readAnalog(0);
        uint16_t Analog1 = readAnalog(1);
        char Analog0_H = Analog0 >> 8;
        char Analog0_L = Analog0;
        char Analog1_H = Analog1 >> 8;
        char Analog1_L = Analog1;
        char sendBuf[7] = {InputOutput,Analog0_H,Analog0_L,Analog1_H,Analog1_L,'\r','\0'};
        //Send ProtocolVariables
        Serial.println(sendBuf);
        client.write(sendBuf);
      }
      if(buf[0] == 0x02){
        Serial.println("write");
       setOutput(buf[1]);
      }
      hotplug();
      //setOutput(outputs);  
      outputs++;
      
  
    }
  }
  
 
  delay(100); 
}

void hotplug(){
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));          
  Wire.write(byte(0x0F));         
  Wire.endTransmission();
  
}

void setOutput(uint8_t output){
  Wire.beginTransmission(0x38); 
  Wire.write(byte(0x01));            
  Wire.write(byte(output<<4));            
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
