/*
 * Draft of potential Robot Code
 * 
 * "Main" function that calls and runs all others
 * 
 * can edit functionality s.t. this keeps track of all the pins
 * or can edit functionallity such taht a menu keeps track of all the pins
 */

 /*
  *  DONT NEED SERIAL COMMUNICATION: 
  *   CHEW MAKES PIN GO HIGH WHILE PICKING UP AN OBJECT
  *   CHEW MAKES PIN GO LOW WHEN FINISHED PICKING UP AN OBJECT
  *   
  *   SOLO MAKES PIN GO HIGH (THEN RESETS) TO INDICATE STATE INCREMENT
  */

#include <phys253.h>
#include "Motion.h"
#include "Crossing.h"

#define rightMotor 0
#define leftMotor 1
#define rightMostQRD 1
#define rightMiddleQRD 2
#define leftMiddleQRD 3
#define leftMostQRD 4

#define baseSpeed 255
#define onTheTape 400
#define overTheCliff 700

#define pGainConst 54
#define dGainConst 0

#define interruptPin 0

void pauseState();
void resumeState();

volatile uint8_t state = 0;
volatile uint8_t rememberState;

Motion hanMovo(rightMotor, leftMotor, onTheTape, baseSpeed);
Crossing hanFlyo(rightMotor, leftMotor, rightMostQRD, leftMostQRD, overTheCliff);

void setup() {
  attachInterrupt(interruptPin, pauseState, RISING);
  attachInterrupt(interruptPin, resumeState, FALLING);
}

void loop() {

  switch (state) {
    case 1 : // STARTING STATE UNTIL FIRST GAP
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);
      //read in QRDs for side cliff detection
      
      if (hanFlyo.cliff()){
        state ++;
        //tell chew to increment State
        //drop the first bridge
      }
      break;
      
    case 2 : //CROSSING THE FIRST GAP
      //read in QRDs to follow bridge
      // need an updated follow tape function that tells us to wiggle if we lose the tape
      //or one that uses all 4, or just being cautious
      //side cliff detection?
      //look for IR beacon
      //when the IR beacon changes from 1k to 10k
        // increment state
        // tell Chew to increment state
      break;
      
    case 3 : //IR BEACON JUST CHANGED FREQUENCY
      //read in QRDs to follow tape
      //Side cliff detection??
      
      if (hanFlyo.cliff()){
        state ++;
        //tell chew to increment State
        //backpedal a certain distance
        // 90 degree turn
      }
      break;

    // COULD ADD ANOTHER CASE HERE AFTER WE HAVE GONE A CERTAIN DISTANCE AND ARE PAST THE STORMTROOPERS
      
    case 4 : //WE JUST LEFT THE TAPE AND TURNED TOWARDS THE SECOND GAP
      //move both wheels forward to cover the same distance
      //Side cliff detection???? maybe not
      
      if (hanFlyo.cliff()){
        state ++;
        //tell chew to increment State
        //drop the second bridge
      }
      break;
      
    case 5 : //WE JUST DROPPED THE SECOND BRIDGE  
      //back to our cautious tape following
      //check for end of tape
      // when our tape ends
      // increment our state, Chew's state
      break;
      
    case 6 : //WE JUST CROSSED OUR SECOND BRIDGE
      //move both wheels forward to cover the same distance
      //when we have moved forward a set distance
      // increment our state, chew's state, 90 deg turn
      break;
      
    case 7 : //WE JUST TURNED TOWARDS THE SECOND TOWER
      //use edge following on the right side
      //(will need chew to look for sonar on left side)
      //either check if interrupt has been called then lift the basket, or have Chew lift the basket
      //if check interrupt method then increment states
      //otherwise wait for basket touch sensor to go to 0 then increment states
      break;
      
    case 8: //COMPLETED THE COURSE MAYBE
      // something about shutting off/ just chilling
      break;
  }
}

// INTERRUPT FUNCTIONS
void pauseState(){
  rememberState = state;
  state = 0;
}

void resumeState(){
  state = rememberState;
}

