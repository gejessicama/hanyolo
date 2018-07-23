/*
   Code for Chew, but just for the first stage of the course
*/

#include "Claw.h"

#define rightTriggerPin 8
#define rightEchoPin 9
#define leftTriggerPin 10
#define leftEchoPin  11

#define clawBaseTurnPin 0
#define clawBaseLiftPin 1
#define clawElbowPin 2
#define clawGripPin 3
#define bridgeDropPin 4


#define frontTriggerPin 12
#define frontEchoPin 13

#define fromSoloPin 2
#define toSoloPin 4
#define dropTheBridgePin 3


const uint16_t objectLimit = 400;

Claw grabbyBoi(objectLimit);

volatile uint8_t state;

boolean readInSonar(uint8_t, uint8_t, uint16_t);
void updateState();
void dropBridge();

void setup() {
//  Serial.begin(9600);
  pinMode(rightTriggerPin, OUTPUT);
  pinMode(leftTriggerPin, OUTPUT);
  pinMode(toSoloPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(fromSoloPin), updateState, RISING);
//  attachInterrupt(digitalPinToInterrupt(dropTheBridgePin), dropBridge, RISING);
}

void loop() {

  switch (state) {
    case 0 : // START BUTTON NOT YET PRESSED
      break;
      
    case 1 : // STARTING STATE UNTIL FIRST GAP
      if (readInSonar(rightTriggerPin, rightEchoPin)) {
        digitalWrite(toSoloPin, HIGH);
        grabbyBoi.pickUpRight();
        digitalWrite(toSoloPin, LOW);
      }
//      if (readInSonar(leftTriggerPin, leftEchoPin)){
//        digitalWrite(toSoloPin, HIGH);
//        grabbyBoi.pickUpLeft();
//        digitalWrite(toSoloPin, LOW);
//      }
      break;
  }
}

boolean readInSonar(uint8_t trig, uint8_t echo){
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10);  
  digitalWrite(trig, LOW);
  return (pulseIn(echo, HIGH) < objectLimit);
}

//  INTERRUPT FUNCTIONS
void updateState() {
  state++;
  Serial.println("updated");
}

void dropBridge(){
  delay(5000);
  // tell servo to turn a certain amount
}


