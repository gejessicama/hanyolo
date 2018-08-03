/*
   All the functions related to crossign the first gap and detecting the IR signal
*/

#include "Crossing.h"

/*
   Initializer for a Crossing Object
*/
Crossing::Crossing(uint8_t rMotor, uint8_t lMotor, uint8_t rQRD, uint8_t lQRD, uint8_t sigIR) {
  rightMotor = rMotor;
  leftMotor = lMotor;
  rightQRD = rQRD;
  leftQRD = lQRD;
  IRsig = sigIR;
}

/*
   Sets necessary constants for Crossing functions. Must be called before anythig else can run
*/
void Crossing::setConstants() {
  overCliff = EEPROM[5] * 10;
  backupTime = EEPROM[6] * 10;
  backupSpeed = EEPROM[7];
}

/*
   Tells us if we are over a cliff
*/
boolean Crossing::cliff() {
  if (analogRead(rightQRD) > overCliff && analogRead(leftQRD) > overCliff) {
    return true;
  } else {
    return false;
  }
}

/*
   code for droping a bridge
*/
void Crossing::dropBridge1(int waitTime, uint8_t servoAngle, double rightWheelPercentage) {

  backUp(rightWheelPercentage);
  motor.speed(rightMotor, 100);
  motor.speed(leftMotor, -100);
  delay(150);
  motor.stop_all();

  delay(waitTime);
  RCServo0.write(servoAngle);
  delay(waitTime);
}

void Crossing::dropBridge2(int waitTime, uint8_t servoAngle, double rightWheelPercentage) {
  backUp(rightWheelPercentage);

  delay(waitTime);
  for (int pos = servoAngle - 10; pos <= servoAngle; pos--) {
    RCServo0.write(pos);
    delay(10);
  }
  delay(waitTime);
}

/*
   Backs up while giving one motor slightly more power
*/
void Crossing::backUp(double rightMotorPercentage) {
  motor.speed(rightMotor, 0);
  motor.speed(leftMotor, backupSpeed);
  delay(backupTime / 2);

  motor.speed(rightMotor, -(backupSpeed) * rightMotorPercentage);
  motor.speed(leftMotor, 0);
  delay(backupTime / 2);

  motor.stop_all();
}

void Crossing::alignStep() {
  motor.speed(leftMotor, backupSpeed);
  motor.speed(rightMotor, backupSpeed);
  delay(backupTime * 1.5);
  motor.stop_all();
}

bool Crossing::detect10KIR() {
  uint8_t val = digitalRead(IRsig);
  if (digitalRead(IRsig) == HIGH) {
    return false;
  } else {
    return true;
  }

}

