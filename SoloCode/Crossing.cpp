/*
 * This will contain the necessary functions for crossing both of the gaps we face
 */

#include <phys253.h>
#include "Crossing.h"

Crossing::Crossing(uint8_t rMotor, uint8_t lMotor, uint8_t rQRD, uint8_t lQRD, uint8_t overCliff){
  rightMotor = rMotor;
  leftMotor = lMotor;
  rightQRD = rQRD;
  leftQRD = lQRD;
  overTheCliff = overCliff;
}

boolean Crossing::cliff(){
    if(analogRead(rightQRD) > overTheCliff || analogRead(leftQRD) > overTheCliff){
      motor.speed(rightMotor, -255);
      motor.speed(leftMotor, -255);
      return true;
    }
    return false;
}

void Crossing::dropBridge1(){
  // basically just backpedal a bit and drop the first bridge
}

void Crossing::dropBridge2(){
  // backpedal a bit and drop the second bridge
}


