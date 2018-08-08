/*
   Time Trial Code for Chew: excludes the claw.
   When a stuffy is seen, chew writes a pin to high waits 3 seconds, writes that pin to low
*/

#include <Servo.h>
#include "ChewConstants.h"

void setup() {
  pinMode(rightLEDPin, OUTPUT);
  pinMode(leftLEDPin, OUTPUT);
  pinMode(toSoloPin, OUTPUT);
}

void loop() {
  if(digitalRead(fromSoloRightPin) == HIGH && readInQSD(rightLEDPin, rightQSDPin)) {
  //if (readInQSD(rightLEDPin, rightQSDPin)) {  // for testing, if we want to ignore the TINAH
    digitalWrite(toSoloPin, HIGH);
    delay(3000);
    //pickUpRight();
    digitalWrite(toSoloPin, LOW);
  }

  if (digitalRead(fromSoloLeftPin) == HIGH && readInQSD(leftLEDPin, leftQSDPin)) {
  //if (readInQSD(leftLEDPin, leftQSDPin)) { // for testing, if we want to ignore the TINAH
    digitalWrite(toSoloPin, HIGH);
    delay(3000);
    //pickUpLeft();
    digitalWrite(toSoloPin, LOW);
  }
}

boolean readInQSD(uint8_t ledPin, uint8_t qsdPin) {
  digitalWrite(ledPin, HIGH);
  delayMicroseconds(readWait);
  onValue = analogRead(qsdPin);
  digitalWrite(ledPin, LOW);
  delayMicroseconds(readWait);
  offValue = analogRead(qsdPin);
  //  Serial.print(onValue);
  //  Serial.print(", ");
  //  Serial.println(offValue);
  return ((offValue - onValue) > objectLimit);
}

// Pick up something from the right
void pickUpRight() {
  startClaw();
  swivel();
  moveClaw(susan, susanRight);
  moveClaw(grip, gripOpen);
  moveClaw(base, baseRightDown);
  moveClaw(elbow, elbowRightDown);
  moveClaw(grip, gripClose);
  swivel();
  dropoff(susanBasketRight);
  travelRight();
  endClaw();
}

// Pickup something from the left
void pickUpLeft() {
  startClaw();
  moveClaw(elbow, 30);
  moveClaw(susan, susanLeft);
  moveClaw(grip, gripOpen);
  moveClaw(base, baseLeftDown);
  moveClaw(elbow, elbowLeftDown);
  moveClaw(grip, gripClose);
  swivel();
  dropoff(susanBasketLeft);
  travelLeft();
  endClaw();
}

// Drops stuff off in the basket
void dropoff(uint8_t side) {
  moveClaw(susan, side);
  moveClaw(base, baseDropoff);
  moveClaw(elbow, elbowDropoff);
  moveClaw(grip, gripOpen);
}

// The position for the claw to be in while we're just running the course
void travelRight() {
  moveClaw(susan, susanTravelRight);
  moveClaw(base, baseTravel);
  moveClaw(elbow, elbowTravel);
  moveClaw(grip, gripClose);
}

void travelLeft() {
  moveClaw(susan, susanTravelLeft);
  moveClaw(base, baseTravel);
  moveClaw(elbow, elbowLeftDown);
  moveClaw(grip, gripClose);
}

// Position before susan moves so that we don't hit anything
void swivel() {
  moveClaw(base, baseSwivel);
  elbow.write(elbowSwivel);
  moveClaw(grip, gripClose);
}

// Moves a servo to a given position, with a delay to allow for smoother movement
void moveClaw(Servo servo, uint8_t pos) {
  servo.write(pos);
  delay(moveDelay);
}

// Attaches all servos, right before movement for power saving
void startClaw() {
  base.attach(clawBasePin);
  elbow.attach(clawElbowPin);
  grip.attach(clawGripPin);
  susan.attach(clawSusanPin);
}

// Detaches all servos, after movement, for power saving
void endClaw() {
  base.detach();
  elbow.detach();
  grip.detach();
  susan.detach();
}
