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

void Crossing::setConstants(){
  overCliff = EEPROM[5] * 10;
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
  backUp(420);delay(2000);  
  RCServo0.write(40);
  delay(2000);
  /*
  digitalWrite(communicationPin, HIGH);
  digitalWrite(communicationPin, LOW);*/
  //wait for bridge to fall
}

void Crossing::backUp(long reverseTime){
  int vback = -255;
  //unsigned long reverseTime = 400.0;
  unsigned long startTime = millis();
  unsigned long endTime = millis();
  while (endTime - startTime < reverseTime){
    motor.speed(rightMotor, vback);
    motor.speed(leftMotor, -vback);
    endTime = millis();
  }
  motor.stop_all();
}
void Crossing::dropBridge2(uint8_t communicationPin) {
  
  backUp(400);
  RCServo0.write(180);
  /*backUp(backUpDistance);
  digitalWrite(communicationPin, LOW);*/
}

uint8_t lval;
bool Crossing::detectIR(){
  uint8_t val = digitalRead(IRsig);
    if (val !=lval){
      //LCD.clear();
      if (val == HIGH){
        //LCD.print("Not 10K");
        return true;
      }else if(val == LOW){
        //LCD.println("10K");
        return false;
      }
    }
  lval = val;
}

