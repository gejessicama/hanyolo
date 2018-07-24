/*
   The Motion Class contains functions that let us follow tape, follow an edge

   Cannot follow both tape and edge at the same time.
   Requires that motors are wired so that positive inputs make them go forwards
*/
#include "Motion.h"

Motion::Motion(uint8_t rMotor, uint8_t lMotor, uint8_t onTape, uint8_t overCliff,
               int v0, double powerMult) {

  rightMotor = rMotor;
  leftMotor = lMotor;
  ON = onTape;
  CLIFF = overCliff;
  baseSpeed = v0;
  powerMultiplier = powerMult;
}

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

  pidControl(proportionalGain, derivativeGain);
}

void Motion::pidControl(uint8_t proportionalGain, uint8_t derivativeGain) {
  proportionalTerm = proportionalGain * currentError;
  derivativeTerm = derivativeGain * (currentError - lastState) * 1.0 / count;
  gain = proportionalTerm + derivativeTerm;

  motor.speed(rightMotor, powerMultiplier * (baseSpeed + gain));
  motor.speed(leftMotor, powerMultiplier * (-baseSpeed + gain));

  if (currentError != lastError) {
    lastState = lastError;
    count = 0;
  }
  lastError = currentError;
  count ++;
}

boolean Motion::isOnWhite (uint8_t qrdPin) {
  if (analogRead(qrdPin) < ON) {
    return true;
  } else {
    return false;
  }
}


