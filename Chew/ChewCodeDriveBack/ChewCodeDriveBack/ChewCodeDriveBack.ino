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
  if (digitalRead(fromSoloRightPin) == HIGH && readInQSD(rightLEDPin, rightQSDPin, 200)) {
  //if (readInQSD(rightLEDPin, rightQSDPin, 200)) {  // for testing, if we want to ignore the TINAH
    digitalWrite(toSoloPin, HIGH);
    pickUpRight();
    //delay(3000);
    digitalWrite(toSoloPin, LOW);
  }

  if (digitalRead(fromSoloLeftPin) == HIGH && readInQSD(leftLEDPin, leftQSDPin, 150)) {
  //if (readInQSD(leftLEDPin, leftQSDPin, 200)) { // for testing, if we want to ignore the TINAH
    digitalWrite(toSoloPin, HIGH);
    pickUpLeft();
    //delay(3000);
    digitalWrite(toSoloPin, LOW);
  }
}

boolean readInQSD(uint8_t ledPin, uint8_t qsdPin, uint8_t objectLimit) {
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
  moveClaw(elbow, elbowRightDown);
  moveClaw(base, baseRightDown);
  moveClaw(grip, gripClose);
  swivel();
  dropoff(susanBasketRight);
  travelRight();
  endClaw();
}

// Pickup something from the left: hold it for a few seconds (to move from under the zipline), then deposit in basket. 
void pickUpLeft() {
  startClaw();
  swivelLow();
  moveClaw(susan, susanLeft);
  moveClaw(grip, gripOpen);
  moveClaw(elbow, elbowLeftDown);
  moveClaw(base, baseLeftDown);
  moveClaw(grip, gripClose);
  
  moveClaw(base, baseLeftDown+50); // hold it a bit above the ground so it doesn't drag and pull out
  digitalWrite(toSoloPin, LOW); // DELAY and allow chassis movement
  delay(2000);
  
  swivelLow();
  dropoff(susanBasketLeft);
  travelRight();
  endClaw();
}

// Drops stuff off in the basket
void dropoff(uint8_t side) {
  moveClaw(susan, side);
  moveClaw(base, baseDropoff);
  moveClaw(elbow, elbowDropoff);
  moveClaw(grip, gripOpen);
}

void dropoffLeft(uint8_t side) {
  moveClaw(susan, side);
  moveClaw(base, baseDropoff);
  moveClaw(elbow, elbowDropoff);
  moveClaw(grip, gripOpen);
}

// The position for the claw to be in while we're just running the course
void travelRight() {
  moveClaw(susan, susanTravelRight);
  moveClaw(base, baseTravelRight);
  moveClaw(elbow, elbowTravelRight);
  moveClaw(grip, gripClose);
}

void travelLeft() {
  moveClaw(susan, susanTravelLeft);
  moveClaw(base, baseTravelLeft);
  moveClaw(elbow, elbowLeftDown);
  moveClaw(grip, gripClose);
}

// Position before susan moves so that we don't hit anything
void swivel() {
  moveClaw(base, baseSwivel);
  elbow.write(elbowHighSwivel);
  moveClaw(grip, gripClose);
}

void swivelLow() {
  moveClaw(base, baseSwivel);
  elbow.write(elbowLowSwivel);
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
  susan.detach();
  base.detach();
  elbow.detach();
  grip.detach();
}
