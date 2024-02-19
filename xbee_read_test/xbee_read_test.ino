#include "SoftwareSerial.h"

SoftwareSerial xbeeSerial(9, 10); // RX, TX
byte discardByte;
boolean xbeeConnect = false;
unsigned long checkMac1 = 0x0013A200;
unsigned long checkMac2 = 0x408BAD45;
unsigned long mac1;
unsigned long mac2;
int frameLen;
int frameType;
int battState;
boolean serialClear = false;

void setup(){
	Serial.begin(9600);
	xbeeSerial.begin(9600);

}

void loop(){
xbeeRead();
if (xbeeConnect){
	Serial.println("Connected");
}else{
  Serial.println("Not Connnected");
}
delay(4000);
}

void xbeeRead(){
  Serial.println("check");
  if (xbeeSerial.available() && xbeeSerial.read() == 0x7E){
	frameLen = xbeeSerial.read();
	frameLen = frameLen << 8;
	frameLen = frameLen | xbeeSerial.read();
	Serial.print("Frame Length: ");
	Serial.println(frameLen, HEX);
	frameType = xbeeSerial.read();
	if (frameType == 0x92){
	  mac1 = xbeeSerial.read();
      for (int i = 1; i < 4; i++){
		mac1 = mac1 << 8;
		mac1 = mac1 | xbeeSerial.read();
	  }
	  mac2 = xbeeSerial.read();
      for (int i = 1; i < 4; i++){
		mac2 = mac2 << 8;
		mac2 = mac2 | xbeeSerial.read();
	  }
	  Serial.print("Mac Address: ");
	  Serial.print(mac1, HEX);
      Serial.println(mac2, HEX);
      if (mac1 == checkMac1 && mac2 == checkMac2){
	    xbeeConnect = true;
		count = 0;
		unlock = true;
		digitalWrite(relayPin[3], HIGH);
		for (int i = 1; i < 8; i++){
		  discardByte = xbeeSerial.read();
		}
		battState = xbeeSerial.read();
		battState = battState << 8;
		battState = battState | xbeeSerial.read();
		Serial.print("Battery State raw: ");
		Serial.println(battState);
		if (battState < 2560){
		  Serial.println("Battery Low");
		  digitalWrite(relayPin[4], HIGH);
		}
		while(xbeeSerial.available() && !serialClear){
		  for(int i = 1; i < (frameLen - 18); i++){
		    discardByte = xbeeSerial.read();
		  }
		  serialClear = true;
		}
		serialClear = false;
	  } else {
	    xbeeConnect = false;
	  }
	} else {
	  for (int i = 1; i < (frameLen + 1); i++){
	    discardByte = xbeeSerial.read();
	  }
	  xbeeConnect = false;
	  }
  } else {
    xbeeConnect = false;
  }
}
