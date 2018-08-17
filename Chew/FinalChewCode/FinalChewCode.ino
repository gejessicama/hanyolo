/*
   Main code for the Arduino (Chewy or Chew)
   Contians the main (loop) function and several helper functions for
    reading in our Ewok sensors and picking up Ewoks with our claw
*/

#include <Servo.h>
#include "ChewConstants.h"

/*
   Sets up certain pins as outputs, all the rest are inputs by default
*/
void setup() {
  pinMode(rightLEDPin, OUTPUT);
  pinMode(leftLEDPin, OUTPUT);
  pinMode(toSoloPin, OUTPUT);
}


/*
   Main function for the Arduino
   Continuously loops until we turn off the power
*/
void loop() {

  /*
     By checking our from Solo Pins, we determine which side of the robot
     we should be looking on to find Ewoks
  */

  if (digitalRead(fromSoloRightPin) == HIGH &&
      readInQSD(rightLEDPin, rightQSDPin, 200)) {

    digitalWrite(toSoloPin, HIGH);
    pickUpRight();
    digitalWrite(toSoloPin, LOW);
  }

  if (digitalRead(fromSoloLeftPin) == HIGH &&
      readInQSD(leftLEDPin, leftQSDPin, 150)) {
    digitalWrite(toSoloPin, HIGH);
    pickUpLeft();
    digitalWrite(toSoloPin, LOW);
  }
}


/*
   Reads in the given Ewok sensor by reading in reflectance and subracting ambient light
*/
boolean readInQSD(uint8_t ledPin, uint8_t qsdPin, uint8_t objectLimit) {
  digitalWrite(ledPin, HIGH);
  delayMicroseconds(readWait);
  onValue = analogRead(qsdPin);

  digitalWrite(ledPin, LOW);
  delayMicroseconds(readWait);
  offValue = analogRead(qsdPin);

  return ((offValue - onValue) > objectLimit);
}


/*
   Sequence of commands to pick up and Ewok on the right side of our robot
*/
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


/*
   Commands for picking up and Ewok on the left of our robot
*/
void pickUpLeft() {
  startClaw();

  swivelLow();
  moveClaw(susan, susanLeft);
  moveClaw(grip, gripOpen);
  moveClaw(elbow, elbowLeftDown);
  moveClaw(base, baseLeftDown);

  moveClaw(grip, gripClose);

  // Because of the course layout, we need to hold onto the stuffy
  moveClaw(base, baseLeftDown + 50);
  digitalWrite(toSoloPin, LOW);

  // and wait a little before droping it off in our basket
  delay(2000);

  swivelLow();
  dropoff(susanBasketLeft);
  travelRight();

  endClaw();
}


/*
   Drops the Ewok in our basket by specifying the dropoff position
*/
void dropoff(uint8_t side) {
  moveClaw(susan, side);
  moveClaw(base, baseDropoff);
  moveClaw(elbow, elbowDropoff);
  moveClaw(grip, gripOpen);
}


/*
   Moves claw to an out-of-the-way position
*/
void travelRight() {
  moveClaw(susan, susanTravelRight);
  moveClaw(base, baseTravelRight);
  moveClaw(elbow, elbowTravelRight);
  moveClaw(grip, gripClose);
}


/*
   Moves claw to an upright position so that we don't hit our robot when we move
*/
void swivel() {
  moveClaw(base, baseSwivel);
  elbow.write(elbowHighSwivel);
  moveClaw(grip, gripClose);
}


/*
   Moves claw to another position so that we won't hit our robot or hit a zipline when we move
*/
void swivelLow() {
  moveClaw(base, baseSwivel);
  elbow.write(elbowLowSwivel);
  moveClaw(grip, gripClose);
}


/*
   Moves the given servo motor to the given position
   The delay is to avoid our claw jerking and moving too quickly
*/
void moveClaw(Servo servo, uint8_t pos) {
  servo.write(pos);
  delay(moveDelay);
}


/*
   Attachs all servo motors so we can use them
*/
void startClaw() {
  base.attach(clawBasePin);
  elbow.attach(clawElbowPin);
  grip.attach(clawGripPin);
  susan.attach(clawSusanPin);
}


/*
   Detaches all servo motors to save power when not in use
*/
void endClaw() {
  susan.detach();
  base.detach();
  elbow.detach();
  grip.detach();
}
