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
int count = 0;
int seq = 0;
long l = 0.0;
long r = 0.0;
long avl = l;
long avr = r;
boolean Crossing::cliff() {
  /*
  long lt = analogRead(leftQRD);
  long rt = analogRead(rightQRD);
  seq++;
  if ( lt> overTheCliff &&  rt> overTheCliff){

    
    l+=lt;
    r+=rt;
    
    if(seq != count){
      count = 0;
      l = 0.0;
      r = 0.0;
      seq = 0;
    }
      
  }
  if (count > 0){
    avl = l/(count);
    avr = r/(count);
  }

  
  if (count == 5 && avl> overTheCliff &&  avr> overTheCliff){
    motor.speed(rightMotor, -255);
    motor.speed(leftMotor, -255);
    motor.stop(rightMotor);
    motor.stop(leftMotor);
    count = 0;
    return true;
  }
  */
  if (analogRead(rightQRD) > overTheCliff && analogRead(leftQRD) > overTheCliff) {
    
    motor.speed(rightMotor, -255);
    motor.speed(leftMotor, -255);
    motor.stop(rightMotor);
    motor.stop(leftMotor);
    return true;
  }
  return false;
}

void Crossing::dropBridge1(uint8_t communicationPin) {
  //move back for some time
  int vback = -200;
  unsigned long reverseTime = 500.0;
  unsigned long startTime = millis();
  unsigned long endTime = millis();
  while (endTime - startTime < reverseTime){
    motor.speed(rightMotor, vback);
    motor.speed(leftMotor, vback);
    endTime = millis();
  }
  
  //backup here
  digitalWrite(communicationPin, HIGH);
  digitalWrite(communicationPin, LOW);
  //wait for bridge to fall
}

void Crossing::dropBridge2(uint8_t communicationPin) {
  backUp(backUpDistance);
  digitalWrite(communicationPin, LOW);
}


