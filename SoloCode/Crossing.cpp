/*
   This will contain the necessary functions for crossing both of the gaps we face

   For dropBridge1() and dropBridge2() the robot backs up to the right distance then
   sends a signal to the Arduino to drop the bridge
*/

#include "Crossing.h"

Crossing::Crossing(uint8_t rMotor, uint8_t lMotor, uint8_t rQRD, uint8_t lQRD, uint8_t overCliff,
                   uint8_t backDist) {
  rightMotor = rMotor;
  leftMotor = lMotor;
  rightQRD = rQRD;
  leftQRD = lQRD;
  overTheCliff = overCliff;
  backUpDistance = backDist;
}

boolean Crossing::cliff() {
  if (analogRead(rightQRD) > overTheCliff || analogRead(leftQRD) > overTheCliff) {
    motor.speed(rightMotor, -255);
    motor.speed(leftMotor, -255);
    return true;
  }
  return false;
}

void Crossing::backUp(uint8_t distance) {
  // monitor both? or one? encoder to back up to that distance
}

void Crossing::dropBridge1(uint8_t communicationPin) {
  backUp(backUpDistance);
  digitalWrite(communicationPin, HIGH);
}

void Crossing::dropBridge2(uint8_t communicationPin) {
  backUp(backUpDistance);
  digitalWrite(communicationPin, LOW);
}


