/*
   All the functions related to crossign the first gap and detecting the IR signal
*/

#include "Crossing.h"
#include "Constants.h"

/*
   Initializer for a Crossing Object
*/
Crossing::Crossing(int val) {
  overCliff = EEPROM[1] * 10;
  backupPowerMult = EEPROM[4] / 100.0;
  rightWheelPercent = EEPROM[5] / 100.0;
  backupTime = EEPROM[6] * 10;
}

/*
   Tells us if we are over a cliff
*/
boolean Crossing::cliff() {
  return (analogRead(rightMostQRD) > overCliff && analogRead(leftMostQRD) > overCliff);
}

/*
   code for droping a bridge
*/
void Crossing::dropBridge1() {

  delay(bridgeDropWaitTime);
  RCServo0.write(firstBridgeServoAngle);
  delay(bridgeDropWaitTime);
  motor.speed(rightMotor, -80);
  motor.speed(leftMotor, 100);
  delay(100);
  motor.stop_all();
  delay(100);
}

bool Crossing::detect10KIR() {
  if (digitalRead(irSignalPin) == HIGH) {
    return false;
  } else {
    return true;
  }
}

