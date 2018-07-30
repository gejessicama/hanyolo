/*
   The Motion Class contains functions that let us follow tape, follow an edge

   Cannot follow both tape and edge at the same time.
   Requires that motors are wired so that positive inputs make them go forwards
*/
#include "Motion.h"

/*
   Initializes the motion object by declaring which pin drives which motor
*/
Motion::Motion(uint8_t rMotor, uint8_t lMotor) {
  rightMotor = rMotor;
  leftMotor = lMotor;
}

/*
   Sets necessary values for all the variables that motion depends on
*/
void Motion::setConstants() {
  baseSpeed = EEPROM[0];
  powerMult = EEPROM[1] / 100.0;
  proportionalGain = EEPROM[2];
  derivativeGain = EEPROM[3];
  onTape = EEPROM[4] * 10;
  overCliff = EEPROM[5] * 10;
  backupTime = EEPROM[6] * 3;
}

//void Motion::reset() {
//  lastError = 0;
//  lastState = 0;
//  lastOn = -1;
//  count = 0;
//
//}

/*
 * Calculates the error values and such for following tape
 */
void Motion::followTape(uint8_t rightQRD, uint8_t leftQRD) {

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

  pidControl();
}

/*
 * Calculates errors and such for following the right edge of a cliff
 */
void Motion::followRightEdge(uint8_t outQRD, uint8_t inQRD) {

  rVal = isOverCliff(outQRD);
  lVal = isOverCliff(inQRD);

  if (rVal && !lVal) { //all is good
    currentError = 0;
    lastState = 0;
  } else if (!rVal) { // too far left
    currentError = -1;
    lastOn = -1;
  } else if (lVal) { // too far right
    currentError = 1;
    lastOn = 1;
  }

  pidControl();
}

/*
 * Controls the motion of the robot based on the last calculated error values and such
 */
void Motion::pidControl() {
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

/*
   Returns true if the given QRD is over a white surface, false otherwise
*/
boolean Motion::isOnWhite (uint8_t qrdPin) {
  if (analogRead(qrdPin) < onTape) {
    return true;
  } else {
    return false;
  }
}

/*
   Returns true if the given QRD is over a cliff, false otherwise
*/
boolean Motion::isOverCliff (uint8_t qrdPin) {
  if (analogRead(qrdPin) > overCliff) {
    return true;
  } else {
    return false;
  }
}


void Motion::driveMotors() {
  motor.speed(rightMotor, baseSpeed * powerMult);
  motor.speed(leftMotor, -baseSpeed * powerMult);
}

