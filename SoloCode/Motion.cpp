/*
   The Motion Class contains functions that let us follow tape, follow an edge

   Cannot follow both tape and edge at the same time. 
   Requires that motors are wired so that positive inputs make them go forwards
*/
#include "Motion.h"

Motion::Motion(uint8_t rMotor, uint8_t lMotor, uint8_t onTape, uint8_t overCliff, int v0){
  
  rightMotor = rMotor;
  leftMotor = lMotor;
  ON = onTape;
  CLIFF = overCliff;
  baseSpeed = v0;
}

void Motion::followTape(uint8_t rightQRD, uint8_t leftQRD, uint8_t proportionalGain,
                        uint8_t derivativeGain) {

  rVal = isOnWhite(rightQRD);
  lVal = isOnWhite(leftQRD);

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

  proportionalTerm = proportionalGain * currentError;
  derivativeTerm = derivativeGain * (currentError - lastState) * 1.0 / count; 
  gain = proportionalTerm + derivativeTerm;
  
  motor.speed(rightMotor, baseSpeed + gain);
  motor.speed(leftMotor, baseSpeed - gain);

  if (currentError != lastError) {
    lastState = lastError;
    count = 0;
  }
  lastError = currentError;
  count ++;
}

void Motion::followRightEdge(uint8_t outQRD, uint8_t inQRD,uint8_t proportionalGain,
                        uint8_t derivativeGain){
 
  rVal = isOverCliff(outQRD);
  lVal = isOnWhite(inQRD);

  if (rVal && !lVal) { // We are over the cliff all is good
    currentError = 0;
    lastState = 0;
  } else if (!rVal && !lVal) { // We are too far left
    currentError = -1;
    lastOn = -1;
  } else if (rVal && lVal) { // We are too far right
    currentError = 1;
    lastOn = 1;
  }

  proportionalTerm = proportionalGain * currentError;
  derivativeTerm = derivativeGain * (currentError - lastState) * 1.0 / count; 
  gain = proportionalTerm + derivativeTerm;
  
  motor.speed(rightMotor, baseSpeed + gain);
  motor.speed(leftMotor, baseSpeed - gain);

  if (currentError != lastError) {
    lastState = lastError;
    count = 0;
  }
  lastError = currentError;
  count ++;
}


//long Motion::getEncoder0(bool velocity,bool displacement){ 
//  long cp = encoder0Pos;
//  long velo;
//  long posi = cp;
//  delay(1);
//  n = digitalRead(encoder0PinA);
//  if ((encoder0PinALast == LOW) && (n == HIGH)) {
//    if (digitalRead(encoder0PinB) == LOW) {
//      encoder0Pos--;
//    } else {
//      encoder0Pos++;
//    }
//    unsigned long ft = millis();
//    //float vel = 1000.0*((float) encoder0Pos-cp)/(ft-it+.5);
//    time_[i%8] = ft;
//    pos[i%8] = cp;
//    float vel = 1000.0*((float)cp - pos[(i+1)%8])/(ft - time_[(i+1)%8]);
//    velo = vel;
//    posi = cp;
//    it = ft;
//    i++;
//  }
//   //nsigned long vel = (encoder0Pos - cp)/(50
//  encoder0PinALast = n;
//  if (velocity){
//      return velo;
//    }
//    if(displacement){
//      return posi;
//    }
//}
//long Motion::getEncoder1(bool velocity,bool displacement){ 
//  long cp = encoder1Pos;
//  long velo;
//  long posi = cp;
//  delay(1);
//  n = digitalRead(encoder1PinA);
//  if ((encoder1PinALast == LOW) && (n == HIGH)) {
//    if (digitalRead(encoder1PinB) == LOW) {
//      encoder1Pos--;
//    } else {
//      encoder1Pos++;
//    }
//    unsigned long ft = millis();
//    //float vel = 1000.0*((float) encoder0Pos-cp)/(ft-it+.5);
//    time_1[i1%8] = ft;
//    pos1[i1%8] = cp;
//    float vel = 1000.0*((float)cp - pos[(i1+1)%8])/(ft - time_[(i1+1)%8]);
//    velo = vel;
//    posi = cp;
//    it = ft;
//    i1++;
//  }
//   //nsigned long vel = (encoder0Pos - cp)/(50
//  encoder1PinALast = n;
//  if (velocity){
//      return velo;
//    }
//    if(displacement){
//      return posi;
//    }
//}

boolean Motion::isOnWhite (uint8_t qrdPin) {
  if (analogRead(qrdPin) < ON)
    return true;
  return false;
}

boolean Motion::isOverCliff (uint8_t qrdPin){
  if(analogRead(qrdPin) > CLIFF)
    return true;
  return false;
}



