/*
   Code for Chew
   Chew will constantly be checking both the left and right sonars. When it finds a stuffy,
   it will store the value of where that stuffy is and send a signal to the TINAH, then go
   back to normal. If the TINAH sends a signal to pick something up, chew will pick up
   in the position of whatever was last seen. The resting state for last seen will be in
   front?? maybe.... but test this thouroughly without the bridges
*/

#include "Claw.h"

#define rightTriggerPin 4
#define rightEchoPin 5
#define leftTriggerPin 6
#define leftEchoPin  7

#define clawSusanPin 8
#define clawBasePin 9
#define clawElbowPin 10
#define clawGripPin 12
#define bridgeDropPin 13

#define fromSoloPin 2
#define toSoloPin 0
#define dropTheBridgePin 3

const uint16_t objectLimit = 400;
const uint16_t stuffyLimit = 60000;

Servo susan, base, elbow, grip;

Claw grabbyBoi(susan, base, elbow, grip);

volatile uint8_t state = 1;
volatile uint8_t sideLastSeen;

boolean readInSonar(uint8_t, uint8_t, uint16_t);
//void pickUpStuffy
void updateState();
void dropBridge();

void setup() {
  pinMode(rightTriggerPin, OUTPUT);
  pinMode(leftTriggerPin, OUTPUT);
  pinMode(toSoloPin, OUTPUT);
  susan.attach(clawSusanPin);
  base.attach(clawBasePin);
  elbow.attach(clawElbowPin);
  grip.attach(clawGripPin);
  //attachInterrupt(digitalPinToInterrupt(fromSoloPin), pickUpStuffy, RISING);
  //  attachInterrupt(digitalPinToInterrupt(dropTheBridgePin), dropBridge, RISING);
  grabbyBoi.swivel();
}

void loop() {

  grabbyBoi.pickUpLeft();
  delay(5000);

//  switch (state) {
//    case 0 : // START BUTTON NOT YET PRESSED
//      break;
//
//    case 1 : // STARTING STATE UNTIL FIRST GAP
//      if (readInSonar(rightTriggerPin, rightEchoPin)) {
//        digitalWrite(toSoloPin, HIGH);
//        grabbyBoi.pickUpRight();
//        digitalWrite(toSoloPin, LOW);
//      }
//      if (readInSonar(leftTriggerPin, leftEchoPin)) {
//        digitalWrite(toSoloPin, HIGH);
//        grabbyBoi.pickUpLeft();
//        digitalWrite(toSoloPin, LOW);
//      }
//      break;
//  }
}

boolean readInSonar(uint8_t trig, uint8_t echo) {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  return (pulseIn(echo, HIGH) > stuffyLimit);
}

//  INTERRUPT FUNCTIONS
void updateState() {
  state++;
  Serial.println("updated");
}

void dropBridge() {
  delay(5000);
  // tell servo to turn a certain amount
}


