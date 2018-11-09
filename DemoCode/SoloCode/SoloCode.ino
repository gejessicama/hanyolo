/*
   Han Solo will move in a circle to the left, for a specified amount of time, 
   the send a signal to the arduino and wait for a return signal, then circle
   to the left over and over again, until it is turned off.
*/

#include <phys253.h>
#include "Constants.h"

void setup() {
  pinMode(fromChewPin, INPUT);
  pinMode(toChewPinLeft, OUTPUT);
  pinMode(toChewPinRight, OUTPUT);
}

void loop() {

circleSpinLeft:
  driveMotors(1, driveRatio);
  delay(circleTime);
  stopMotors();

clawDanceLeft:
  digitalWrite(toChewPinLeft, HIGH);
  while(digitalRead(fromChewPin) == LOW){}

circleSpinRight:
  driveMotors(driveRatio, 1);
  delay(circleTime);
  stopMotors();

clawDanceRight:
  digitalWrite(toChewPinRight, HIGH);
  while(digitalRead(fromChewPin) == LOW){}

}

/*
   Drives both motors forward at a constant rate
*/
void driveMotors(double rightMultiplier, double leftMultiplier) {
  motor.speed(rightMotor, baseDrivingSpeed * rightMultiplier);
  motor.speed(leftMotor, -baseDrivingSpeed * leftMultiplier);
}

void stopMotors() {
  motor.speed(rightMotor, -255);
  motor.speed(leftMotor, 255);
  motor.stop_all();
}



