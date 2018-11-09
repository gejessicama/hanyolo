#include <Servo.h>
#include "ChewConstants.h"

/* Sets up certain pins as outputs, all the rest are inputs by default */
void setup() {
  pinMode(rightLEDPin, OUTPUT);
  pinMode(leftLEDPin, OUTPUT);
  pinMode(toSoloPin, OUTPUT);
}

void loop() {
  if (digitalRead(fromSoloRightPin) == HIGH){
    digitalWrite(toSoloPin, LOW);
    danceOne();
    digitalWrite(toSoloPin, HIGH);
    delay(50);
    digitalWrite(toSoloPin, LOW);
  }

  if (digitalRead(fromSoloLeftPin) == HIGH){
    digitalWrite(toSoloPin, LOW);
    danceTwo();
    digitalWrite(toSoloPin, HIGH);
    delay(50);
    digitalWrite(toSoloPin, LOW);
  }
}

void danceOne() {
  startClaw();
  swivel();
  moveClaw(grip, gripOpen);
  moveClaw(grip, gripClose);
  moveClaw(susan, susanRight);
  moveClaw(elbow, elbowRightDown);
  moveClaw(grip, gripOpen);
  moveClaw(grip, gripClose);
  endClaw();
}

void danceTwo() {
  startClaw();
  swivel();
  moveClaw(grip, gripOpen);
  moveClaw(grip, gripClose);
  moveClaw(susan, susanLeft);
  moveClaw(elbow, elbowLeftDown);
  moveClaw(grip, gripOpen);
  moveClaw(grip, gripClose);
  endClaw();
}

void swivel() {
  moveClaw(base, baseSwivel);
  elbow.write(elbowHighSwivel);
  moveClaw(grip, gripClose);
}

void moveClaw(Servo servo, uint8_t pos) {
  servo.write(pos);
  delay(moveDelay);
}

void startClaw() {
  base.attach(clawBasePin);
  elbow.attach(clawElbowPin);
  grip.attach(clawGripPin);
  susan.attach(clawSusanPin);
}

void endClaw() {
  susan.detach();
  base.detach();
  elbow.detach();
  grip.detach();
}
