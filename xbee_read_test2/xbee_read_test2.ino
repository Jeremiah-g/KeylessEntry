#include "SoftwareSerial.h"

SoftwareSerial xbeeSerial(9, 10); // RX, TX
int xbeeData;

void setup(){
	Serial.begin(9600);
	xbeeSerial.begin(9600);

}

void loop(){
if (xbeeSerial.available()){
	xbeeData = xbeeSerial.read();
	Serial.println(xbeeData, HEX);
	xbeeData = xbeeData << 8;
	Serial.println(xbeeData, HEX);
	xbeeData = xbeeData + xbeeSerial.read();
	Serial.println(xbeeData, HEX);
	xbeeData = xbeeData << 8;
	Serial.println(xbeeData, HEX);
	xbeeData = xbeeData + xbeeSerial.read();
	Serial.println(xbeeData, HEX);
	xbeeData = xbeeData << 8;
	Serial.println(xbeeData, HEX);
	xbeeData = xbeeData + xbeeSerial.read();
	Serial.println(xbeeData, HEX);
	xbeeData = xbeeData << 8;
	Serial.println(xbeeData, HEX);
	xbeeData = xbeeData + xbeeSerial.read();
	Serial.println(xbeeData, HEX);
	xbeeData = xbeeData << 8;
	Serial.println(xbeeData, HEX);
	xbeeData = xbeeData + xbeeSerial.read();
	Serial.println(xbeeData, HEX);
	xbeeData = xbeeData << 8;
	Serial.println(xbeeData, HEX);
	xbeeData = xbeeData + xbeeSerial.read();
	Serial.println(xbeeData, HEX);
	xbeeData = xbeeData << 8;
	Serial.println(xbeeData, HEX);
	xbeeData = xbeeData + xbeeSerial.read();
	Serial.println(xbeeData, HEX);
	delay(1000);
	}
}
