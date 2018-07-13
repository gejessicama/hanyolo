 #include <phys253.h>

 /*
  * Function draft for PID Tape Following with 4 QRDS
  * Function draft for front cliffSensing with 4 QRDS
  */

#define rightMotor 2
#define leftMotor 3
#define rightMostQRD 4
#define rightMidQRD 3
#define leftMidQRD 2
#define leftMostQRD 1
// right/left defined as looking at the robot

const int ON = 100;
 
void setup()
{
    #include <phys253setup.txt>
}

// some of these should not be ints?
int rVal, lVal;
int kp, kd;
int p,d,g;
int currentErr, lastErr, lastState;
int lastOn = -1;
int v0 = 255;

float count = 1;
int displayCount =0;

void loop() {
 
//
//  kp = knob(6)/1024.0*200;
//  kd = knob(7)/1024.0*200;
//
//  if (displayCount == 100){
//    LCD.clear();
//    LCD.print(lVal);
//    LCD.print(" ");
//    LCD.print(rVal);
////    LCD.print(" ");
////    LCD.print(currentErr);
//    displayCount = 0;
//  }
//
//  p = kp*currentErr;
//  d = kd*(currentErr - lastState)/count; // this is never zero
//  g = p + d; 
//
//  motor.speed(rightMotor, v0 + g);
//  motor.speed(leftMotor, -v0 + g);
//
//  if (currentErr != lastErr){
//    lastState = lastErr;
//    count = 0;
//  }
//  lastErr = currentErr;
//  count ++;
//  displayCount ++;
}

/*
 * givens: a qrd pin to read
 * returns: if the QRD is on or off the tape
 */
boolean isOnTape (uint8_t qrdPin){
  if (analogRead(qrdPin) > ON)
    return true;
  return false;
}

/*
 * givens: the 2 QRD pins to read, and the previous error
 * returns: the error in the robots position (- too left, + too right)
 */
int getTapeW2(uint8_t rightQRD, uint8_t leftQRD, int prevErr){
  bool rVal = isOnTape(rightQRD);
  bool lVal = isOnTape(leftQRD);
  
  if (rVal && lVal)
    return 0;
  if (!rVal && lVal)
    return 1;
  if (rVal && !lVal)
    return -1;
  return 5*prevErr;
}

/*
 * givens: the 4 QRD pins to read
 * returns: the error in the robots position (- too left, + too right)
 */
int getTapeW4(uint8_t rrQrd, uint8_t rmQrd, uint8_t lmQrd, uint8_t llQrd){
  bool rrVal = isOnTape(rrQrd);
  bool rmVal = isOnTape(rmQrd);
  bool lmVal = isOnTape(lmQrd);
  bool llVal = isOnTape(llQrd);
  
  if (rmVal && lmVal)
    return 0;
  if (!rmVal && lmVal && !llVal)
    return 1;
  if (rmVal && !lmVal && !rrVal)
    return -1;
  if (llVal)
    return 5;
  return -5;
}

/*
 * givens: all 4 qrd pins in front
 * returns: true if they are all looking at a cliff, false otherwise
 */
boolean atCliff (uint8_t rrQrd, uint8_t rmQrd, uint8_t lmQrd, uint8_t llQrd){
  if (isOnTape(rrQrd) && isOnTape(rmQrd) && isOnTape(lmQrd) && isOnTape(llQrd))
    return true;
  return false;
}


