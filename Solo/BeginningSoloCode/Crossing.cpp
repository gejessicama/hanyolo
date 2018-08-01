/*
   This will contain the necessary functions related to crossing the first gap
*/

#include "Crossing.h"

/*
 * Initializer for a Crossing Object
 */
Crossing::Crossing(uint8_t rMotor, uint8_t lMotor, uint8_t rQRD, uint8_t lQRD, uint8_t sigIR) {
  rightMotor = rMotor;
  leftMotor = lMotor;
  rightQRD = rQRD;
  leftQRD = lQRD;
  IRsig = sigIR;
}

/*
 * Sets necessary constants for Crossing functions. Must be called before anythig else can run
 */
void Crossing::setConstants(){
  overCliff = EEPROM[5] * 10;
  backupTime = EEPROM[6] * 10;
  backupSpeed = EEPROM[7];
}


//int count = 0;
//int seq = 0;
//long l = 0.0;
//long r = 0.0;
//long avl = l;
//long avr = r;

boolean Crossing::cliff() {
  if (analogRead(rightQRD) > overCliff && analogRead(leftQRD) > overCliff) {
    motor.speed(rightMotor, -255);
    motor.speed(leftMotor, 255);
    motor.stop_all();
    return true;
  }
  return false;
}

void Crossing::dropBridge(int waitTime, uint8_t servoAngle) {
  backUp();
//  while(true){

//  }
  delay(waitTime);  
       for (int pos = servoAngle - 10; pos <= servoAngle; pos += 1) {
    RCServo0.write(pos);              
    delay(10);                       
  }
  //RCServo0.write(servoAngle);
  delay(waitTime);

  motor.speed(leftMotor, -200);
  motor.speed(rightMotor,  200);
  delay(250);
}

void Crossing::backUp(){
    motor.speed(leftMotor, backupSpeed);
    motor.speed(rightMotor, 0);
    delay(backupTime/2.0);
    motor.speed(rightMotor, -(backupSpeed)*0.6);
    motor.speed(leftMotor, 0);
    delay(backupTime/2.0);
//    motor.speed(rightMotor, -255);
//    motor.speed(leftMotor, 255);
//    delay(backupTime);
    motor.stop(rightMotor);
    motor.stop(leftMotor);

    
//  int vback = -255;
//  unsigned long reverseTime = 400.0;
//  unsigned long startTime = millis();
//  unsigned long endTime = millis();
//  while (endTime - startTime < reverseTime){
//    motor.speed(rightMotor, vback);
//    motor.speed(leftMotor, -vback);
//    endTime = millis();
//  }
//  motor.stop_all();

  
}


bool Crossing::detect10KIR(){
  uint8_t val = digitalRead(IRsig);
      //LCD.clear();
      if (val == HIGH){
        //LCD.print("not10K");
        return false;
      }else if(val == LOW){
        //LCD.println("10K");
        return true;
      }
}

