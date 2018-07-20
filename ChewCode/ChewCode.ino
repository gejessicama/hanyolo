/*
   Code for the Arduino

   Chew keeps track of what state of the course we are in, this state is updated
   everytime we recieve a signal from Solo

   Depending on the state we look for an Ewok and then tell Solo that we found
   an Ewok and go pick it up, and tell Solo we finished picking it up

   Chew also controls the claw (AND HOPEFULLY ALSO THE SCISSOR LIFT)
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

volatile uint8_t state = 0;

boolean readInSonar(uint8_t, uint8_t, uint16_t);
void updateState();
void dropBridge1();
void dropBridge2();

void setup() {
  pinMode(rightTriggerPin, OUTPUT);
  pinMode(leftTriggerPin, OUTPUT);
  pinMode(frontTriggerPin, OUTPUT);
  pinMode(toSoloPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(fromSoloPin), updateState, RISING);
  attachInterrupt(digitalPinToInterrupt(dropTheBridgePin), dropBridge1, RISING);
  attachInterrupt(digitalPinToInterrupt(dropTheBridgePin), dropBridge2, FALLING);
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
      break;
      
    case 2 : // CROSSING THE FIRST GAP
      if (readInSonar(rightTriggerPin, rightEchoPin)) {
        digitalWrite(toSoloPin, HIGH);
        grabbyBoi.pickUpRight();
        digitalWrite(toSoloPin, LOW);
      }
      break;

    case 3 : // IR BEACON JUST CHANGED FREQUENCY
      break;
    
    case 4 : // WE ARE NOW PAST THE STORMTROOPERS
      if (readInSonar(leftTriggerPin, leftEchoPin)) {
        digitalWrite(toSoloPin, HIGH);
        grabbyBoi.pickUpLeft();
        digitalWrite(toSoloPin, LOW);
      }
      break;

    // COULD ADD ANOTHER CASE HERE FOR WHEN WE ARE PAST THE STORMTROOPERS
      
    case 5 : // WE JUST LEFT THE TAPE AND TURNED TOWARDS THE SECOND GAP
      break;
      
    case 6 : // WE JUST DROPPED THE SECOND BRIDGE
      break;
      
    case 7 : // WE JUST CROSSED OUR SECOND BRIDGE
      if (readInSonar(frontTriggerPin, frontEchoPin)) {
        digitalWrite(toSoloPin, HIGH);
        grabbyBoi.pickUpFront();
        digitalWrite(toSoloPin, LOW);
      }
      break;
      
    case 8 : // WE JUST TURNED TOWARDS THE SECOND TOWER
      if (readInSonar(leftTriggerPin, leftEchoPin)) {
        digitalWrite(toSoloPin, HIGH);
        grabbyBoi.pickUpLeft();
        digitalWrite(toSoloPin, LOW);
      }
      break;
    case 9 : // COMPLETED THE COURSE MAYBE?
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
}

void dropBridge1(){
  // tell servo to turn a certain amount
}

void dropBridge2(){
  // tell servo to turn an additional amount
}


