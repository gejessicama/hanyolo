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
  backUpSpeed = EEPROM[7];
  turningTime = EEPROM[9] * 10;
}

/*
   Resets PID values so that new tape following doesnt deal with old stuff
*/
void Motion::reset(uint8_t last) {
  lastError = 0;
  lastState = 0;
  lastOn = last;
  // WATCH OUT FOR THIS
  count = 0;

}

bool Motion::findTape(uint8_t rightQRD, uint8_t leftQRD, unsigned int searchTime) {
  double mult = 0.5;
  if (!isOnWhite(rightQRD) || !isOnWhite(leftQRD)) {
    return true;
  }
  //turns left first, is possible to go too far left
  unsigned long startTime = millis();
  motor.speed(rightMotor, powerMult * baseSpeed * mult);
  motor.speed(leftMotor, powerMult * baseSpeed * mult);
  while ((millis() < startTime + searchTime / 2)) {
    if (!isOnWhite(rightQRD) || !isOnWhite(leftQRD)) {
      motor.stop_all();
      return true;
    }
  }
  //turns right second, is possible to go too far right
  startTime = millis();
  motor.speed(rightMotor, -powerMult * baseSpeed * mult);
  motor.speed(leftMotor, -powerMult * baseSpeed * mult);
  while ((millis() < startTime + searchTime)) {
    if (!isOnWhite(rightQRD) || !isOnWhite(leftQRD)) {
      motor.stop_all();
      return true;
    }
  }
  return false;
}

bool Motion::findRightEdge(uint8_t outQRD, uint8_t inQRD, unsigned int searchTime) {
  double multr = 0.45;
  double multl = 0.9;
  if (isOverCliff(outQRD) && !isOverCliff(inQRD)) {
    return true;
  }
  else if (!isOverCliff(outQRD)) {
    unsigned long startTime = millis();
    motor.speed(rightMotor, powerMult * baseSpeed * multr);
    motor.speed(leftMotor, -powerMult * baseSpeed * multl);
    while (millis() < startTime + searchTime) {
      if (isOverCliff(outQRD)) {
        motor.speed(leftMotor, 255);
        motor.speed(rightMotor, -255);
        motor.stop_all();
        return true;
      }
    }
  }
  //  else{
  //    unsigned long startTime = millis();
  //    motor.speed(rightMotor, powerMult * baseSpeed * mult);
  //    motor.speed(leftMotor, powerMult * baseSpeed * mult);
  //    while (millis() < startTime + searchTime) {
  //      if (!isOverCliff(inQRD)) {
  //        motor.stop_all();
  //        return true;
  //      }
  //    }
  //  }
  return false;
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
    //lastOn = -1 * lastOn; // this will cause us to sweep??? Maybe
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
   Drives both motors forward at a constant rate
*/
void Motion::driveMotors(double multiplier) {
  motor.speed(rightMotor, baseSpeed * multiplier);
  motor.speed(leftMotor, -baseSpeed * multiplier);
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
   Tells the robot to exectute a right turn by stopping the left wheel and backing up the right wheel
*/
void Motion::turnRight() {
  motor.speed(rightMotor, -backUpSpeed);
  motor.speed(leftMotor, -backUpSpeed);
  delay(turningTime);
  motor.stop_all();
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

