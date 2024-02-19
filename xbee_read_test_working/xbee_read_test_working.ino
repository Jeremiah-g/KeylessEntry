#include <SoftwareSerial.h>

SoftwareSerial xbeeSerial(8, 9); // RX, TX
byte discardByte;
boolean xbeeConnect = false;
boolean unlock =  false;
int checkMSB = 0xAD;
int checkLSB = 0x45;
int count = 10;

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
      xbeeConnect = true;
	  count = 0;
    }else{
      xbeeConnect = false;
    }
    while (xbeeSerial.available()){
      discardByte = xbeeSerial.read();
    }
}else{
  xbeeConnect = false;
}
}else{
  xbeeConnect  = false;
}
    if (xbeeConnect){
      Serial.println("Connected");
    }else{
      Serial.println("Not Connected");
	  count += 1;
    }
	
if (count > 10){
	Serial.println("Locked");
	unlock = false;
}else{
	Serial.println("Unlocked");
	}
delay (4000);
}
