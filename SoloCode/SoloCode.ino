/*
   Code for the TINAH

   Solo
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
#define rightOutQRD 5
#define rightInQRD 6

#define baseSpeed 255
#define onTheTape 400
#define overTheCliff 700

#define pGainConst 54
#define dGainConst 0

#define fromChewPin 0
#define toChewPin 8
#define rightEncoderPin 1
#define leftEncoderPin 3

volatile uint8_t state = 0;
volatile uint8_t rememberState;
volatile uint16_t rightWheelDist, leftWheelDist;

Motion hanMovo(rightMotor, leftMotor, onTheTape, overTheCliff, baseSpeed);
Crossing hanFlyo(rightMotor, leftMotor, rightMostQRD, leftMostQRD, overTheCliff);

void updateChewState();

//  INTERRUPT FUNCTIONS
void pauseState();
void resumeState();
void incrementRightPos(); //just need to know how much distance corresponds to
void incrementLeftPos();

void setup() {
  attachInterrupt(digitalPinToInterrupt(fromChewPin), pauseState, RISING);
  attachInterrupt(digitalPinToInterrupt(fromChewPin), resumeState, FALLING);
}

void loop() {

  switch (state) {
    case 1 : // STARTING STATE UNTIL FIRST GAP :: could also read in QRDs to detect cliffs on the side of the robot
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);
      if (hanFlyo.cliff()) {
        state ++;
        updateChewState();
        hanFlyo.dropBridge1(); // dropping the first bridge will include backing up to the right distance
      }
      break;

    case 2 : //CROSSING THE FIRST GAP :: could have updated tape function to wiggle if we lose the tape, side cliff detection

      // THIS NEEDS TO BE MORE THOUGH OUT
      /*
         Second bridge should block IR signal until we have turned a bit (and hopefully picked up the stuffy
         as soon as we detect the IR signal we need to stop and wait for it to change frequencies
         likely we should have a helper function we call that just loops and waits for signal to change
         and when the signal changes then we exit and increment
      */
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);

      //look for IR beacon
      //when the IR beacon changes from 1k to 10k
      state++;
      updateChewState();
      attachInterrupt(digitalPinToInterrupt(rightEncoderPin), incrementRightPos, RISING);
      break;

    case 3 : //IR BEACON JUST CHANGED FREQUENCY
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);
      //Side cliff detection??

      // WHEN WE HAVE GONE A CERTAIN DISTANCE CHANGE STATE
      state++;
      updateChewState();
      detachInterrupt(digitalPinToInterrupt(rightEncoderPin));
      break;

    case 4 : // WE ARE NOW PAST THE STORMTROOPERS
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);
      if (hanFlyo.cliff()) {
        state ++;
        updateChewState();
        //hanFlyo.backUp();
        //hanFlyo.turnRight();
        attachInterrupt(digitalPinToInterrupt(rightEncoderPin), incrementRightPos, RISING);
      }
      break;

    case 5 : //WE JUST LEFT THE TAPE AND TURNED TOWARDS THE SECOND GAP
      //move both wheels forward to cover the same distance
      //Side cliff detection???? maybe not

      if (hanFlyo.cliff()) {
        state ++;
        updateChewState();
        detachInterrupt(digitalPinToInterrupt(rightEncoderPin));
        hanFlyo.dropBridge2();
      }
      break;

    case 6 : //WE JUST DROPPED THE SECOND BRIDGE
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);
      if (hanFlyo.cliff()){
        state++;
        updateChewState();
        attachInterrupt(digitalPinToInterrupt(rightEncoderPin), incrementRightPos, RISING);
      }
      break;

    case 7 : //WE JUST CROSSED OUR SECOND BRIDGE
      //move both wheels forward to cover the same distance
      //when we have moved forward a set distance
        state++;
        updateChewState();
        detachInterrupt(digitalPinToInterrupt(rightEncoderPin));
        
      break;

    case 8 : //WE JUST TURNED TOWARDS THE SECOND TOWER
      hanMovo.followRightEdge(rightOutQRD, rightInQRD, pGainConst, dGainConst);
      //either check if interrupt has been called then lift the basket, or have Chew lift the basket
      //if check interrupt method then increment states
      //otherwise wait for basket touch sensor to go to 0 then increment states
      break;

    case 9 : //COMPLETED THE COURSE MAYBE
      // something about shutting off/ just chilling
      break;
  }
}

void updateChewState() {
  digitalWrite(toChewPin, HIGH);
  digitalWrite(toChewPin, LOW);
}

// INTERRUPT FUNCTIONS
void pauseState() {
  rememberState = state;
  state = 0;
}

void resumeState() {
  state = rememberState;
}

void incrementRightPos(){
  
}

void incrementLeftPos(){
  
}

