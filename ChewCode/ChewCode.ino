/*
   Code for the Arduino

   Chew keeps track of what state of the course we are in, this state is updated
   everytime we recieve a signal from Solo

   Depending on the state we look for an Ewok and then tell Solo that we found
   an Ewok and go pick it up, and tell Solo we finished picking it up

   Chew also controls the claw (AND HOPEFULLY ALSO THE SCISSOR LIFT)
*/

#include "Claw.h"

#define triggerPinRight 8
#define echoPinRight 9
#define triggerPinLeft 10
#define echoPinLeft  11
#define triggerPinFront 12
#define echoPinFront 13

#define interruptPin 0
#define toSoloPin 1
#define objectLimit 400

volatile uint8_t state = 0;

boolean readInSonar(uint8_t, uint8_t, uint16_t);
void updateState();

void setup() {
  pinMode(triggerPinRight, OUTPUT);
  pinMode(triggerPinLeft, OUTPUT);
  pinMode(triggerPinFront, OUTPUT);
  pinMode(toSoloPin, OUTPUT);
  attachInterrupt(interruptPin, updateState, RISING);
}

void loop() {

  switch (state) {
    case 0 : // START BUTTON NOT YET PRESSED
      break;
      
    case 1 : // STARTING STATE UNTIL FIRST GAP
      if (readInSonar(triggerPinRight, echoPinRight, objectLimit)) {
        digitalWrite(toSoloPin, HIGH);
        //activate right claw pick up
        digitalWrite(toSoloPin, LOW);
      }
      break;
      
    case 2 : // CROSSING THE FIRST GAP
      if (readInSonar(triggerPinRight, echoPinRight, objectLimit)) {
        digitalWrite(toSoloPin, HIGH);
        //activate right claw pick up
        digitalWrite(toSoloPin, LOW);
      }
      break;
      
    case 3 : // IR BEACON JUST CHANGED FREQUENCY
      //ignore for a certain amount of hard coded time
      // (we need to pass the arch and the stormtroopers)
      if (readInSonar(triggerPinLeft, echoPinLeft, objectLimit)) {
        digitalWrite(toSoloPin, HIGH);
        //activate left claw pick up
        digitalWrite(toSoloPin, LOW);
      }
      break;

    // COULD ADD ANOTHER CASE HERE FOR WHEN WE ARE PAST THE STORMTROOPERS
      
    case 4 : // WE JUST LEFT THE TAPE AND TURNED TOWARDS THE SECOND GAP
      break;
      
    case 5 : // WE JUST DROPPED THE SECOND BRIDGE
      break;
      
    case 6 : // WE JUST CROSSED OUR SECOND BRIDGE
      if (readInSonar(triggerPinFront, echoPinFront, objectLimit)) {
        digitalWrite(toSoloPin, HIGH);
        //activate front claw pick up
        digitalWrite(toSoloPin, LOW);
      }
      break;
      
    case 7 : // WE JUST TURNED TOWARDS THE SECOND TOWER
      if (readInSonar(triggerPinLeft, echoPinLeft, objectLimit)) {
        digitalWrite(toSoloPin, HIGH);
        //activate left claw pick up
        digitalWrite(toSoloPin, LOW);
      }
      break;
    case 8 : // COMPLETED THE COURSE MAYBE?
      break;
      
  }
}

boolean readInSonar(uint8_t trig, uint8_t echo, uint16_t objLim){
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10);  
  digitalWrite(trig, LOW);
  return (pulseIn(echo, HIGH) < objLim);
}

//  INTERRUPT FUNCTION
void updateState() {
  state++;
}


