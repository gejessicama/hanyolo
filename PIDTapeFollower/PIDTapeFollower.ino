 #include <phys253.h>

 /*
  * weakness: assumes if both off that the last state was one off
  * weakness: assumes all variables are ints
  * weakness: positive is the same direction for both motors right now, can change the direction
  * weakness: g is unchecked, need to keep it from being too big by doing math
  * weakness: even if we are on the tape, we still have a derivative term 
  *   from when we were last off the tape
  *   
  * weakness: begins with lastOn being the right
  * 
  * need to pick some base power, then add/subtract the error control from that
  * to keep the robot moving forward
  */

#define rightMotor 2
#define leftMotor 3
#define rightQRD 3
#define leftQRD 1
// right/left defined as looking at the robot

// change based on what sensors read for on and off the tape
// cover sensors with a sheild 
//(full sheild and can lift sensors up further for more linear reading)
#define ON 100
 
void setup()
{
    #include <phys253setup.txt>
    Serial.begin(9600); 
    Serial.println("Hello"); 
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
  rVal = analogRead(rightQRD);
  lVal = analogRead(leftQRD);

//  Serial.print(lVal);
//  Serial.print(" ");
//  Serial.print(rVal);
//  Serial.print(" ");
  
  if (rVal > ON && lVal > ON){ // this may be mixed up
    currentErr = 0;
    lastState = 0;
//    LCD.clear();
//    LCD.print("Both ON!");
  }else if (rVal > ON && lVal < ON){
    currentErr = -1; 
    lastOn = -1;
//    LCD.clear();
//    LCD.print("Left Off");
  }else if (rVal <= ON && lVal > ON){
    currentErr = 1; 
    lastOn = 1;
//    LCD.clear();
//    LCD.print("Right Off");
  }else{
    currentErr = lastOn*5;
//    LCD.clear();
//    LCD.print("Both Off");
  }

  kp = knob(6)/1024.0*200;
  kd = knob(7)/1024.0*200;

  if (displayCount == 100){
    LCD.clear();
    LCD.print(lVal);
    LCD.print(" ");
    LCD.print(rVal);
//    LCD.print(" ");
//    LCD.print(currentErr);
    displayCount = 0;
  }

  p = kp*currentErr;
  d = kd*(currentErr - lastState)/count; // this is never zero
  g = p + d; 

  motor.speed(rightMotor, v0 + g);
  motor.speed(leftMotor, -v0 + g);

  if (currentErr != lastErr){
    lastState = lastErr;
    count = 0;
  }
  lastErr = currentErr;
  count ++;
  displayCount ++;
}
