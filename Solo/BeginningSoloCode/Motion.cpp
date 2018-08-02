/*
   Contains functions related to motion: tapeFollowing, edgeFollowing, driving forwards, stopping the motors
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
}

/*
   Resets PID values so that new tape following doesnt deal with old stuff
*/
void Motion::reset() {
  lastError = 0;
  lastState = 0;
  lastOn = -1;
  count = 0;

}

/*
   Calculates the error values and such for following tape
*/
void Motion::followTape(uint8_t rightQRD, uint8_t leftQRD) {

  bool rVal = isOnWhite(rightQRD);
  bool lVal = isOnWhite(leftQRD);

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
   Uses all four QRDs to follow tape
*/
void Motion::followTapeFour(uint8_t rightMostQRD, uint8_t rightMidQRD, uint8_t leftMostQRD, uint8_t leftMidQRD) {
  bool rrVal = isOnWhite(rightMostQRD);
  bool rmVal = isOnWhite(rightMidQRD);
  bool lmVal = isOnWhite(leftMidQRD);
  bool llVal = isOnWhite(leftMostQRD);

  if (!rmVal && !lmVal) { // We are on the tape and all is good
    currentError = 0;
    lastState = 0;
  } else if (!rmVal) { // We are too far left
    currentError = -1;
    lastOn = -1;
  } else if (!lmVal) { // We are too far right
    currentError = 1;
    lastOn = 1;
  } else if (!rrVal) { // Way too far left
    currentError = -5;
    lastOn = -1;
  } else if (!llVal) { // Way too far right
    currentError = 5;
    lastOn = 1;
  } else { // OH BOY I DON'T EVEN KNOW WHERE I AM
    currentError = 10 * lastOn;
    lastOn = -1 * lastOn; // this will cause us to sweep??? Maybe
  }

  pidControl();
}

/*
   Calculates errors and such for following the right edge of a cliff
*/
void Motion::followRightEdge(uint8_t outQRD, uint8_t inQRD) {

  bool rVal = isOverCliff(outQRD);
  bool lVal = isOverCliff(inQRD);

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

//boolean findTape(uint8_t rightMostQRD, uint8_t rightMidQRD, uint8_t leftMidQRD, uint8_t leftMostQRD){
//  /*
//   * if the middle ones are over the tape, no problem
//   * if the two right ones are over the tape, too far left
//   * if the two left ones are over the tape, too far right
//   * if only the right most is over the tape, way far left
//   * if only the left most is over the tape, way far right
//   * if none are on the tape, sweep to the right, try again
//   * if none are on the tape, sweep to the left, try again
//   */
//
//   rrVal = isOnTape
//
//}

/*
   Controls the motion of the robot based on the last calculated error values and such
*/
void Motion::pidControl() {
  int proportionalTerm = proportionalGain * currentError;
  int derivativeTerm = derivativeGain * (currentError - lastState) * 1.0 / count;
  int gain = proportionalTerm + derivativeTerm;

  motor.speed(rightMotor, powerMult * (baseSpeed + gain));
  motor.speed(leftMotor, powerMult * (-baseSpeed + gain));

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
   Drives both motors forward at a constant rate
*/
void Motion::driveMotors() {
  motor.speed(rightMotor, baseSpeed * powerMult);
  motor.speed(leftMotor, -baseSpeed * powerMult);
}

/*
   Hard stop to motors
*/
void Motion::stopMotors() {
  motor.speed(rightMotor, -255);
  motor.speed(leftMotor, 255);
  motor.stop_all();
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

