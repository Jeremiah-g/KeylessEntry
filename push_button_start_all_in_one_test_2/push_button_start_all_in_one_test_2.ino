#include "OneButton.h"
#include "avr/io.h"
#include "avr/interrupt.h"
#include "SoftwareSerial.h"

SoftwareSerial xbeeSerial(8, 9);
int bttnPin = 10;
OneButton btnIn(bttnPin, true);
unsigned char relayPin[5] = {4, 5, 6, 7, 3}; //pins for relays
boolean acc = false;
boolean ign = false;
boolean egn = false;
unsigned long checkMac1 = 0x0013A200;
unsigned long checkMac2 = 0x408BAD45;
unsigned long mac1;
unsigned long mac2;
int frameLen;
int frameType;
int battState;
boolean serialClear = false;
volatile boolean xbeeConnect = false;
volatile boolean unlock = false;
volatile byte discardByte;
volatile int count = 10;
volatile int xbeeStartBit;

void setup() {
  Serial.begin(9600);
  xbeeSerial.begin(9600);
  //set button pin
  btnIn.attachClick(btnClickFunction);
  btnIn.attachPress(btnPressFunction);
  //set relay pins
  for (int i = 0; i < 5; i++) {
    pinMode(relayPin[i], OUTPUT);
  }
  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B

  TIMSK1 = (1 << TOIE1);
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  sei();          // enable global interrupts
}

void loop() {
  while (unlock) {
    btnIn.tick();
  }
}
void btnClickFunction() {
  if (acc == false) {
    acc = true;
    digitalWrite(relayPin[0], HIGH);
    ign = true;
    digitalWrite(relayPin[1], HIGH);
    Serial.println("ACC ON, IGN ON");
  }
  else if (acc == true && egn == true) {
    egn = false;
    ign = false;
    digitalWrite(relayPin[1], LOW);
    Serial.println("ENGINE OFF, IGN OFF");
  }
  else {
    allOff();
  }
}
void allOff() {
  acc = false;
  ign = false;
  egn = false;
  for (int i = 0; i < 2; i++) {
    digitalWrite(relayPin[i], LOW);
  }
  Serial.println("All OFF");
}

void btnPressFunction() {
  if (acc == true) {
    if (ign == false) {

      ign = true;
      digitalWrite(relayPin[1], HIGH);
    }
    digitalWrite(relayPin[0], LOW);
    digitalWrite(relayPin[2], HIGH);
    while (digitalRead(bttnPin) == LOW) {
    }
    egn = true;
    digitalWrite(relayPin[2], LOW);
    digitalWrite(relayPin[0], HIGH);
    Serial.println("ENGINE ON");
  }
}
ISR(TIMER1_OVF_vect) {
  Serial.println("check");
  Serial.print("Serial Length: ");
  Serial.println(xbeeSerial.available());
  if (xbeeSerial.available() >= 22) {
    xbeeStartBit = xbeeSerial.read();
    Serial.print("Xbee Start Bit: ");
    Serial.println(xbeeStartBit, HEX);
    if (xbeeStartBit == 0x7E) {
      frameLen = xbeeSerial.read();
      frameLen = frameLen << 8;
      frameLen = frameLen | xbeeSerial.read();
      Serial.print("Frame Length: ");
      Serial.println(frameLen, HEX);
      frameType = xbeeSerial.read();
      if (frameType == 0x92) {
        mac1 = xbeeSerial.read();
        for (int i = 1; i < 4; i++) {
          mac1 = mac1 << 8;
          mac1 = mac1 | xbeeSerial.read();
        }
        mac2 = xbeeSerial.read();
        for (int i = 1; i < 4; i++) {
          mac2 = mac2 << 8;
          mac2 = mac2 | xbeeSerial.read();
        }
        Serial.print("Mac Address: ");
        Serial.print(mac1, HEX);
        Serial.println(mac2, HEX);
        if (mac1 == checkMac1 && mac2 == checkMac2) {
          xbeeConnect = true;
          count = 0;
          unlock = true;
          digitalWrite(relayPin[3], HIGH);
          for (int i = 1; i < 8; i++) {
            discardByte = xbeeSerial.read();
          }
          battState = xbeeSerial.read();
          battState = battState << 8;
          battState = battState | xbeeSerial.read();
          Serial.print("Battery State raw: ");
          Serial.println(battState);
          if (battState < 0x0AE8) {
            Serial.println("Battery Low");
            digitalWrite(relayPin[4], HIGH);
          } else {
            digitalWrite(relayPin[4], LOW);
          }
          while (xbeeSerial.available() > 0) {
            discardByte = xbeeSerial.read();
          }
        } else {
          xbeeConnect = false;
        }
      } else {
        while (xbeeSerial.available() > 0) {
          discardByte = xbeeSerial.read();
        }
        xbeeConnect = false;
      }

    } else {
      while (xbeeSerial.available() > 0) {
        discardByte = xbeeSerial.read();
      }
      xbeeConnect = false;
    }
  } else {
    while (xbeeSerial.available() > 0) {
      discardByte = xbeeSerial.read();
    }
    xbeeConnect = false;
  }
  if (xbeeConnect) {
    Serial.println("Connected");
  } else {
    Serial.println("Not Connected");
    count += 1;
  }

  if (count > 20) {
    allOff();
  } else if (count > 4) {
    Serial.println("Locked");
    unlock = false;
    digitalWrite(relayPin[3], LOW);
    digitalWrite(relayPin[4], LOW);
  } else {
    Serial.println("Unlocked");
  }
}

