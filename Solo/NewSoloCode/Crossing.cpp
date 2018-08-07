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
}

void Crossing::dropBridge2(uint16_t backupTime) {
  delay(bridgeDropWaitTime);

  RCServo0.write(secondBridgeServoAngle);
  delay(bridgeDropWaitTime);
  motor.speed(rightMotor, -80);
  motor.speed(leftMotor, 100);
  delay(backupTime);
  motor.stop_all();
  delay(bridgeDropWaitTime);


}

/*
   Backs up while giving one motor slightly more power
*/
void Crossing::backUp(double rightMotorPercentage) {
  motor.speed(rightMotor, 0);
  motor.speed(leftMotor, baseDrivingSpeed * backupPowerMult);
  delay(backupTime / 2);

  motor.speed(rightMotor, -baseDrivingSpeed * backupPowerMult * rightMotorPercentage);
  motor.speed(leftMotor, 0);
  delay(backupTime / 2);

  motor.stop_all();
}

//void Crossing::alignStep() {
//  motor.speed(leftMotor, backupSpeed);
//  motor.speed(rightMotor, backupSpeed);
//  delay(backupTime * 1.5);
//  motor.stop_all();
//}

bool Crossing::detect10KIR() {
  return(!digitalRead(irSignalPin));
}

