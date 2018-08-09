/*
   Contains functions related to motion: tapeFollowing, edgeFollowing, driving forwards, stopping the motors
*/
#include "Motion.h"
#include "Constants.h"

/*
   Initializes the motion object and saves all the needed menu values
*/
Motion::Motion(int val) {
  onTape = EEPROM[0] * 10;
  overCliff = EEPROM[1] * 10;
  regularPowerMult = EEPROM[2] / 100.0;
  slowPowerMult = EEPROM[3] / 100.0;
  backupPowerMult = EEPROM[4] / 100.0;
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

bool Motion::findTapeLeft(uint16_t searchTime) {
  if (!isOnWhite(rightMiddleQRD) || !isOnWhite(leftMiddleQRD)) {
    return true;
  }
  
  driveMotors(slowPowerMult, -slowPowerMult);
  uint32_t startTime1 = millis();
  while (millis() < (startTime1 + (searchTime / 1.5))) {
    if (!isOnWhite(rightMiddleQRD) || !isOnWhite(leftMiddleQRD)) {
      motor.stop_all();
      return true;
    }
  }
    
  driveMotors(-slowPowerMult, slowPowerMult);
  uint32_t startTime2 = millis();
  while (millis() < (startTime2 + searchTime)) {
    if (!isOnWhite(rightMiddleQRD) || !isOnWhite(leftMiddleQRD)) {
      motor.stop_all();
      return true;
    }
    return false;
  }
}

bool Motion::findTapeRight(uint16_t searchTime) {
  if (!isOnWhite(rightMiddleQRD) || !isOnWhite(leftMiddleQRD)) {
    return true;
  }

  driveMotors(-slowPowerMult, slowPowerMult);
  uint32_t startTime1 = millis();
  while (millis() < (startTime1 + searchTime)) {
    if (!isOnWhite(rightMiddleQRD) || !isOnWhite(leftMiddleQRD)) {
      motor.stop_all();
      return true;
    }
  }
  
  driveMotors(slowPowerMult, -slowPowerMult);
  uint32_t startTime2 = millis();
  while (millis() < (startTime2 + searchTime * 2)) {
    if (!isOnWhite(rightMiddleQRD) || !isOnWhite(leftMiddleQRD)) {
      motor.stop_all();
      return true;
    }
  }
  return false;
}

/*
   if all four qrds are reading white, then I want to find tape depending looking to the right
*/
void Motion::lostAndFindTape() {
  if (isOnWhite(rightMostQRD) && isOnWhite(rightMiddleQRD) && isOnWhite(leftMiddleQRD) && isOnWhite(leftMostQRD)) {
    driveMotors(slowPowerMult, slowPowerMult);
    delay(300);
    findTapeRight(lostAndFoundTime);
  }
}

/*
   Calculates the error values and such for following tape
*/
void Motion::followTape(double powerMult) {

  bool rVal = isOnWhite(rightMiddleQRD);
  bool lVal = isOnWhite(leftMiddleQRD);

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

  pidControl(pGainTapeFollowing, dGainTapeFollowing, powerMult);
}


/*
   Controls the motion of the robot based on the last calculated error values and such
*/
void Motion::pidControl(uint8_t proportionalGain, uint8_t derivativeGain, double powerMult) {
  int proportionalTerm = proportionalGain * currentError;
  int derivativeTerm = derivativeGain * (currentError - lastState) * 1.0 / count;
  int gain = proportionalTerm + derivativeTerm;

  motor.speed(rightMotor, powerMult * (baseDrivingSpeed + gain));
  motor.speed(leftMotor, powerMult * (-baseDrivingSpeed + gain));

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
void Motion::driveMotors(double rightMultiplier, double leftMultiplier) {
  motor.speed(rightMotor, baseDrivingSpeed * rightMultiplier);
  motor.speed(leftMotor, -baseDrivingSpeed * leftMultiplier);
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

