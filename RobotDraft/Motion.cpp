/*
   This file will include functions for general motion like tape following,
   edge following, 90 deg turn
*/

#include <phys253.h>
#include "Motion.h"

//  VARIALBLES FOR ALL FUNCTIONS
uint8_t ON, rightMotor, leftMotor, baseSpeed;

//  VARIABLES FOR TAPE FOLLOWING
uint8_t currentError, lastError, lastState, lastOn;
int rVal, lVal;
int proportionalTerm, derivativeTerm, gain;
uint16_t count;

// HELPER FUNCTION DEFINITION
//boolean isOnWhite (uint8_t);

/*
   Constructor for Motion class object
   givens: Motor Pins, what is consitered on the tape, a base speed for the motors
*/
Motion::Motion(uint8_t rMotor, uint8_t lMotor, uint8_t onTape, int v0)
{
  rightMotor = rMotor;
  leftMotor = lMotor;
  ON = onTape;
  baseSpeed = v0;
}

/*
   Follows tape
   givens: the qrdpins to read, proportional and derivative gains for PID control
*/
void Motion::followTape(uint8_t rightQRD, uint8_t leftQRD, uint8_t proportionalGain,
                        uint8_t derivativeGain) {

  rVal = isOnWhite(rightQRD);
  lVal = isOnWhite(leftQRD);

  if (!rVal && !lVal) { // We are on the tape and all is good
    currentError = 0;
    lastState = 0;
  } else if (!rVal && lVal) { // We are too far left
    currentError = -1;
    lastOn = -1;
  } else if (rVal && !lVal) { // We are too far right
    currentError = 1;
    lastOn = 1;
  } else { // Both are off the tape
    currentError = lastOn * 5;
  }

  proportionalTerm = proportionalGain * currentError;
  derivativeTerm = derivativeGain * (currentError - lastState) * 1.0 / count; 
  gain = proportionalTerm + derivativeTerm;
  
  motor.speed(rightMotor, baseSpeed + gain);
  motor.speed(leftMotor, -baseSpeed + gain);

  if (currentError != lastError) {
    lastState = lastError;
    count = 0;
  }
  lastError = currentError;
  count ++;
}

/*
   Tells us if the given QRD is over a reflective surface or not
   givens: the qrd pin to read
*/
boolean Motion::isOnWhite (uint8_t qrdPin) {
  if (analogRead(qrdPin) < ON)
    return true;
  return false;
}

