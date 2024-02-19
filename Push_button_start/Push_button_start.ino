#include <SoftwareSerial.h> //includes SoftwareSerial library
#include "OneButton.h"

const int TxPin = 8; //pin out to display
OneButton btnIn(3, false);
unsigned char relayPin[4] = {4,5,6,7}; //pins for relays
boolean acc = false;
boolean ign = false;
boolean egn = false;
boolean change = true;

//define serial for display
SoftwareSerial mySerial = SoftwareSerial(255, TxPin); // RX, TX

void setup()
{
  //setup serial pin
  pinMode(TxPin, OUTPUT);
  digitalWrite(TxPin, HIGH);
  //set button pin
  btnIn.attachClick(btnClickFunction);
  btnIn.attachDoubleClick(btnDubleClickFunction);
  btnIn.attachDuringLongPress(btnPressFunction);
  //set relay pins
  int i;
  for(i = 0; i < 4; i++)
  {
    pinMode(relayPin[i], OUTPUT);
  }

}

void loop()
{
  btnIn.tick();
  if (change == true) {
    updateDisplay();
  }
  
}

void btnClickFunction(){
  int i;
  if (acc == false){
    acc = true;
    digitalWrite(relayPin[0], HIGH);
    ign = true;
    digitalWrite(relayPin[1], HIGH);
  }
  else {
     acc = false;
     ign = false;
     egn = false;
       for(i = 0; i < 3; i++)
    {
      digitalWrite(relayPin[i],LOW);
    }
  }
  change = true;
}

void btnDubleClickFunction()
{
 if (egn == true){
   egn = false;
   ign = false;
   digitalWrite(relayPin[1],LOW);
 }  
 change = true;
}

void btnPressFunction()
{
  if (acc == true){
  if (ign == false){
    ign = true;
    digitalWrite(relayPin[1], HIGH);
  }
  digitalWrite(relayPin[3], HIGH);
  egn = true;
  delay(1500);
  digitalWrite(relayPin[3], LOW);
  change = true;
}
}
boolean updateDisplay()
{
  if (acc == true){
    mySerial.write(22);
    mySerial.write(17);
    mySerial.write(148);
    mySerial.print("ON ");
  }
  else{
    mySerial.write(21);
    mySerial.write(18);
    mySerial.write(148);
    mySerial.print("OFF");
  }
  if (ign == true){
    mySerial.write(154);
    mySerial.print("ON ");
  }
  else
  {
    mySerial.write(154);
    mySerial.print("OFF");
  }
  if (egn == true){
    mySerial.write(160);
    mySerial.print("ON ");
  }
  else
  {
    mySerial.write(160);
    mySerial.print("OFF");
  }
  
  return change = false;
}
