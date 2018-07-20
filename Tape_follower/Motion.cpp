#include "INCLUDES.h"

#define rightMotor 0
#define leftMotor 1
#define rightQRD 2
#define leftQRD 3
#define lQRD2 4
#define rQRD2 1
#define encoder0PinA 8
#define encoder0PinB 9
#define encoder1PinA 10
#define encoder1PinB 11
#define POW 0.6
#define qrdcliff 400
#define ON 400

int currentErr, lastErr, lastState;
int lastOn = -1;
const int v0 = 200;

float count = 1;
int displayCount = 0;

int encoder0Pos = 0, encoder1Pos = 0;
int encoder0PinALast = LOW, encoder1PinALast = LOW;
int n = LOW;

unsigned long it = millis();
unsigned long time_[8];
long pos[8];
unsigned long time_1[8];
long pos1[8];
int i = 0, i1 = 0;

Motion::Motion(int pr, int dv) : kp(pr), kd(dv) {}

void Motion::followTape() {
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
  /*
    LCD.clear();
    LCD.print("p");
    LCD.print(kp);
    LCD.print("d");
    LCD.print(kd);
    LCD.print("r=");
    LCD.print(rVal);
    LCD.print(" l=");
    LCD.print(lVal);
  */

  int p = kp * currentErr;
  int d = kd * (currentErr - lastState) / count; // this is never zero
  int g = p + d;

  motor.speed(rightMotor, POW * (v0 + g));
  motor.speed(leftMotor, POW * (-v0 + g));
  int v1 = v0 + g;
  int v2 = -v0 + g;

  if (currentErr != lastErr) {
    lastState = lastErr;
    count = 0;
  }
  lastErr = currentErr;
  count ++;
}

bool Motion::cliff() {
  int lVal2 = analogRead(lQRD2);
  int rVal2 = analogRead(rQRD2);
  if (lVal2 > qrdcliff && rVal2 > qrdcliff) {
    //dropBridge1();
    return true;
  }
}

void Motion::dropBridge1() {
  LCD.clear();
  LCD.print("B1 down");
  //motor.speed(rightMotor, POW*(0));
  //motor.speed(leftMotor, POW*(0));
  //delay(1000);
  LCD.clear();
}

void Motion::dropBridge2() {
  LCD.clear();
  LCD.print("B2 down");
  //motor.speed(rightMotor, POW*(0));
  //motor.speed(leftMotor, POW*(0));
  //delay(1000);
  LCD.clear();
}

void Motion::stopMoving() {
  motor.speed(rightMotor, POW * (0));
  motor.speed(leftMotor, POW * (0));
}


bool Motion::reverse(long finalL, long finalR, long currentLeftEncoder, long currentRightEncoder) {
  long posR = getEncoder0(false, true);
  long posL = getEncoder1(false, true);
  if ( currentLeftEncoder <= finalL && currentRightEncoder <= finalR) {
    return true;
  }
  else {
    LCD.clear();
    LCD.print("PR ");
    LCD.print(posR);
    LCD.print("  ");
    LCD.print("PL ");
    LCD.print(posL);

    motor.speed(rightMotor, POW * (-v0));
    motor.speed(leftMotor, POW * (v0));
    reverse(finalL, finalR, posL, posR);
  }
}


long Motion::getEncoder0(bool velocity, bool displacement) {
  long cp = encoder0Pos;
  long velo;
  long posi = cp;
  delay(1);
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
    } else {
      encoder0Pos++;
    }
    unsigned long ft = millis();
    //float vel = 1000.0*((float) encoder0Pos-cp)/(ft-it+.5);
    time_[i % 8] = ft;
    pos[i % 8] = cp;
    float vel = 1000.0 * ((float)cp - pos[(i + 1) % 8]) / (ft - time_[(i + 1) % 8]);
    velo = vel;
    posi = cp;
    it = ft;
    i++;
  }
  //nsigned long vel = (encoder0Pos - cp)/(50
  encoder0PinALast = n;
  if (velocity) {
    return velo;
  }
  if (displacement) {
    return -1 * posi;
  }
}


long Motion::getEncoder1(bool velocity, bool displacement) {
  long cp = encoder1Pos;
  long velo;
  long posi = cp;
  delay(1);
  n = digitalRead(encoder1PinA);
  if ((encoder1PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder1PinB) == LOW) {
      encoder1Pos--;
    } else {
      encoder1Pos++;
    }
    unsigned long ft = millis();
    //float vel = 1000.0*((float) encoder0Pos-cp)/(ft-it+.5);
    time_1[i1 % 8] = ft;
    pos1[i1 % 8] = cp;
    float vel = 1000.0 * ((float)cp - pos[(i1 + 1) % 8]) / (ft - time_[(i1 + 1) % 8]);
    velo = vel;
    posi = cp;
    it = ft;
    i1++;
  }
  //nsigned long vel = (encoder0Pos - cp)/(50
  encoder1PinALast = n;
  if (velocity) {
    return velo;
  }
  if (displacement) {
    return posi;
  }
}


