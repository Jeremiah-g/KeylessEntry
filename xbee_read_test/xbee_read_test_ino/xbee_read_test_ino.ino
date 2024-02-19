#include "SoftwareSerial.h"

SoftwareSerial xbeeSerial(9, 10); // RX, TX
byte discardByte;
boolean xbeeConnected =  false;
int checkMSB = 0xAD;
int checkLSB = 0x45;

void setup(){
  Serial.begin(9600);
  xbeeSerial.begin(9600);
}

void loop(){
if (xbeeSerial.available()){
  if (xbeeSerial.read() == 0x7E){
    for (int i = 1; i < 10; i++){
      discardByte = xbeeSerial.read();
    }
    int macMSB = xbeeSerial.read();
    Serial.print(macMSB, HEX);
    int macLSB = xbeeSerial.read();
    Serial.println(macLSB, HEX);
    if (macMSB == checkMSB && macLSB == checkLSB){
      xbeeConnected = true;
    }else{
      xbeeConnected = false;
    }
    if (xbeeConnected){
      Serial.println("Connected");
    }else{
      Serial.println("Not Connected");
    }
    while (xbeeSerial.available()){
      discardByte = xbeeSerial.read();
    }
}
}
delay (4000);
}
