#include "INCLUDES.h"

#define rightMotor 0
#define leftMotor 1
#define rightQRD 3
#define leftQRD 2
#define ON 100
#define POW 1

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
  
    //int kp = knob(6) / 1024.0 * 200;
    //int kd = knob(7) / 1024.0 * 200;

    LCD.clear();
    LCD.print("kp= ");
    LCD.print(kp);
    LCD.print(" kd= ");
    LCD.print(kd);
  
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
