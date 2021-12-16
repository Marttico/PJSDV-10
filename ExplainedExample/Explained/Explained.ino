// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
uint8_t input = 0;

void setup()
{
  Wire.begin(0x38);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop()
{
  delay(100);
  Serial.println(input);
  if((input >> 4)&0x01){
    Serial.print("A0");
    digitalWrite(A0,HIGH);
  }else{
    digitalWrite(A0,LOW);
  }
  if((input >> 5)&0x01){
    Serial.print("A1");
    digitalWrite(A1,HIGH);
  }else{
    digitalWrite(A1,LOW);
  }
  if((input >> 6)&0x01){
    Serial.print("A1");
    digitalWrite(A2,HIGH);
  }else{
    digitalWrite(A2,LOW);
  }
  if((input >> 7)&0x01){
    Serial.print("A3");
    digitalWrite(A3,HIGH);
  }else{
    digitalWrite(A3,LOW);
  }

  
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(0 < Wire.available()) // loop through all but the last
  {
    //Serial.println("msg");
    char c = Wire.read();
    if(c == 0x01){
      //Serial.println("request");
      input = Wire.read();
    }
    if(c == 0x00){
      Wire.write(input);
      
    }
    //Serial.print(c);        
  }

  
  /*int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer*/
}
