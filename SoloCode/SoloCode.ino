/*
 * Draft of potential Robot Code
 * 
 * "Main" function that calls and runs all others
 * 
 * can edit functionality s.t. this keeps track of all the pins
 * or can edit functionallity such taht a menu keeps track of all the pins
 */

#include <phys253.h>
#include "Motion.h"
#include "Crossing.h"

#define rightMotor 0
#define leftMotor 1
#define rightQRD 8
#define leftQRD 9

#define onTheSurface 400
#define baseSpeed 255

#define pGainConst 54
#define dGainConst 0

void setup() {
  Serial1.begin(9600);
}

uint8_t state = 1;

Motion hanYolo(rightMotor, leftMotor, onTheSurface, baseSpeed);

void loop() {
  /*
     Based on the state, call different functions that will tell the robot how to move
     each case for each state will likely have another non repeated function to call
     when the sate needs to end and switch to the next state

     Most if not all functions called here can be placed in additional classes to make
     organization and debugging easier
  */
  switch (state) {
    case 1 : // STARTING STATE UNTIL FIRST GAP
      hanYolo.followTape(rightQRD, leftQRD, pGainConst, dGainConst);
      //read in QRDs determine speed
      //read in QRDs for cliff detection
      //check if sonar sees a cliff
      //check if sonar sees a stuffy
      // pick up stuffy when seen
      // figure out conditions that lead to state 2
      break;
    case 2 : //we need to cross the gap
      //read in QRDs to follow bridge
      //read in sonar to not run off bridge
      break;
    case 3 : //we are now in front of the IR beacon
      //read in QRDs to follow tape
      //check if sonar sees a cliff
      //check if IR sensor sees the right frequency
      // go go go when right frequency found!!! and next state
      break;
    case 4 : // we are now in the stormtrooper room
      //read in QRDs to tape follow
      //read in QRDs for cliff detection
      //check if sonar sees a stuffy
      // pick up stuffy when found
      // 90 turn when cliff reached
      break;
    case 5 : // we are about to cross the second gap
      // run motors but no tape following
      // check QRDs for cliff
      // check sonar for cliff
      // when front cliff found drop bridge and continue
      // figure out when done crossing 90 turn and to enter next state
      break;
    case 6 : // we just reached first tower
      //check sonar to pick up stuffy
      //use edge following to move forward
      //check sonar to know we have reached 2nd tower
      break;
    case 7 : //we are on the second tower
      // move slowly
      // check sonar (in front??) to find chewy
      // when chewy picked up exectute basket return
      break;
    case 8: //completed course
      // something about shutting off/ just chilling
      break;
  }

}
