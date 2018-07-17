#include "INCLUDES.h"

#define rightMotor 0
#define leftMotor 1
#define rightQRD 2
#define leftQRD 3
#define lQRD2 4
#define rQRD2 1
#define ON 400
#define POW 0.6
#define qrdcliff 400

int currentErr, lastErr, lastState;
int lastOn = -1;
const int v0 = 255;

float count = 1;
int displayCount = 0;

Motion::Motion(int pr,int dv) : kp(pr),kd(dv){}	

void Motion::followTape(){
    int rVal = analogRead(rightQRD);
    int lVal = analogRead(leftQRD);
    
    if (rVal > ON && lVal > ON) { // this may be mixed up
      currentErr = 0;
      lastState = 0;
    } else if (rVal > ON && lVal < ON) {
      currentErr = -1;
      lastOn = -1;
    } else if (rVal <= ON && lVal > ON) {
      currentErr = 1;
      lastOn = 1;
    } else {
      currentErr = lastOn * 5;
    }
    
  
    kp = knob(6) / 1024.0 * 200;
    kd = knob(7) / 1024.0 * 200;

    LCD.clear();
    LCD.print("p");
    LCD.print(kp);
    LCD.print("d");
    LCD.print(kd);
    LCD.print("r=");
    LCD.print(rVal);
    LCD.print(" l=");
    LCD.print(lVal);
    
  
    int p = kp * currentErr;
    int d = kd * (currentErr - lastState) / count; // this is never zero
    int g = p + d;
  
    motor.speed(rightMotor, POW*(v0 + g));
    motor.speed(leftMotor, POW*(-v0 + g));
    int v1 = v0+g;
    int v2 = -v0+g;
  
    if (currentErr != lastErr) {
      lastState = lastErr;
      count = 0;
    }
    lastErr = currentErr;
    count ++; 
}

bool Motion::cliff(){
    int lVal2 = analogRead(lQRD2);
    int rVal2 = analogRead(rQRD2);
    if(lVal2 >qrdcliff && rVal2 > qrdcliff){
       dropBridge1();
    }
}

void Motion::dropBridge1(){
  LCD.clear();
  LCD.print("B1 down");
  motor.speed(rightMotor, POW*(0));
  motor.speed(leftMotor, POW*(0));
  delay(1000);
  LCD.clear();
}

void Motion::dropBridge2(){
  LCD.clear();
  LCD.print("B2 down");
  motor.speed(rightMotor, POW*(0));
  motor.speed(leftMotor, POW*(0));
  delay(1000);
  LCD.clear();
}


