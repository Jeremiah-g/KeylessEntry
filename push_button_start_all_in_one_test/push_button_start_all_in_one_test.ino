#include "SoftwareSerial.h" //includes SoftwareSerial library
#include "OneButton.h" //includes One button library from https://github.com/mathertel/OneButton

// variables for the push button
const int btnPin = 3; 
OneButton btnIn(3, false);

//variables for debouncing the button
volatile long lastDebounceTime = 0; 
long debounceDelay = 100; 
int btnState = LOW;
int lastButtonState = LOW;
int relayPin[4] = {4,5,6,7}; //pins for relays

boolean acc = false;
boolean ign = false;
boolean egn = false;
boolean change = true;

void setup() {
  Serial.begin(9600);
  pinMode(btnPin, INPUT);
  attachInterrupt(1, button_press, RISING);

  //set button pin
  btnIn.attachClick(btnClickFunction);
  btnIn.attachDoubleClick(btnDubleClickFunction);
  btnIn.attachPress(btnPressFunction);
  //set relay pins
  int i;
  for(i = 0; i < 4; i++)
  {
    pinMode(relayPin[i], OUTPUT);
  }
  

}

void loop(){
  
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

void btnDubleClickFunction(){
  if (egn == true){
    egn = false;
    ign = false;
    digitalWrite(relayPin[1],LOW);
 }  
  change = true;
}

void btnPressFunction(){
  if (acc == true){
    if (ign == false){
      ign = true;
      digitalWrite(relayPin[1], HIGH);
    }
    digitalWrite(relayPin[3], HIGH);
	while(btnPin == HIGH){
      delay(100);
	}
    egn = true;
    digitalWrite(relayPin[3], LOW);
    change = true;
  }
}

void button_press(){
  detachInterrupt(1);
  if (millis() - lastDebounceTime > debounceDelay){
    
  btnState = !lastButtonState;
  if (btnState == HIGH){
    Serial.println("Button was pressed.");
	btnIn.tick();
    lastButtonState = btnState;
  }
  else if (btnState == LOW){
    Serial.println("Button was released.");
	btnIn.tick();
    lastButtonState = btnState;
  }
  lastDebounceTime = millis();
  }
    btnState = digitalRead(btnPin);
    if (btnState == HIGH){
     attachInterrupt(1, button_press, FALLING);
  }
  else if (btnState == LOW){
    attachInterrupt(1, button_press, RISING);
  }
}
