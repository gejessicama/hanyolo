/*
   Solo code just for the first stage of the course with full functionality
*/

#include <phys253.h>
#include "Motion.h"
#include "Crossing.h"

#define rightMotor 0
#define leftMotor 1
#define scissorLiftMotor 2

#define rightMostQRD 1
#define rightMiddleQRD 2
#define leftMiddleQRD 3
#define leftMostQRD 4
#define rightOutQRD 5
#define rightInQRD 6

//#define basketSensorPin
//#define scissorUpLimitPin
//#define scissorDownLimitPin
//#define oneIRPin
//#define tenIRPin

#define rightEncoderPin 1
#define leftEncoderPin 2

#define rightEncoderPinA 0
#define rightEncoderPinB 1
#define leftEncoderPinA 2
#define leftEncoderPinB 3


#define fromChewPin 3
#define toChewPin 8
#define dropTheBridgePin 9


#define baseSpeed 255
#define powerMultiplier 0.40 
#define onTheTape 400
#define overTheCliff 650

#define pGainConst 54
#define dGainConst 0

#define distanceToStormtroopers 36
#define backUpBridgeDistance 2

volatile uint8_t state = 0;
volatile uint8_t rememberState;
volatile uint16_t rightWheelDist, leftWheelDist;

Motion hanMovo(rightMotor, leftMotor, onTheTape, overTheCliff, baseSpeed, powerMultiplier,leftEncoderPinA,leftEncoderPinB,rightEncoderPinA,rightEncoderPinB);
Crossing hanFlyo(rightMotor, leftMotor, rightMostQRD, leftMostQRD, overTheCliff, backUpBridgeDistance);

//  HELPER FUNCTIONS
void updateState();
void raiseBasket();
void lowerBasket();

//  INTERRUPT FUNCTIONS
void changeState();
void incrementRightPos(); //just need to know how much distance corresponds to
void incrementLeftPos(); //need to make sure wheels only turn forwards

void setup() {
  //#include <phys253setup.txt>
  //  pinMode(rightEncoderPin, INPUT);
  //  pinMode(leftEncoderPin, INPUT);
  LCD.begin();
  LCD.print("Setup");
//  pinMode(rightEncoderPinA, INPUT);
//  pinMode(rightEncoderPinB, INPUT);
//  pinMode(leftEncoderPinA, INPUT);
//  pinMode(leftEncoderPinB, INPUT);
  
  pinMode(fromChewPin, INPUT);
  pinMode(toChewPin, OUTPUT);
  attachInterrupt(fromChewPin, changeState, CHANGE);//change does not work
//  attachInterrupt(leftEncoderPinA,getEncoderLeftPosHere,RISING);
//  attachInterrupt(rightEncoderPinA,getEncoderRightPosHere,RISING);
}
long startTime = millis();

void loop() {
  /*
  Serial.print("lout ");
  Serial.print(analogRead(leftMostQRD));
  Serial.print(" Rout ");
  Serial.print(analogRead(rightMostQRD));
  Serial.print(" lmid ");
  Serial.print(analogRead(leftMiddleQRD));
  Serial.print(" Rmid ");
  Serial.println(analogRead(rightMiddleQRD));
  *//*
  int lPos = hanMovo.encoderRightPos;
  int rPos = hanMovo.encoderLeftPos;
  hanMovo.nL = digitalRead(leftEncoderPinA);
  hanMovo.nR = digitalRead(rightEncoderPinA);
  
  //int lPos = hanMovo.getEncoderLeftPos();
  //int rPos = hanMovo.getEncoderRightPos();

  long timeNow = millis();

  if (timeNow - startTime >= 1000.0){
    startTime = timeNow;
    LCD.clear();
    LCD.print(" L "); 
    LCD.print(hanMovo.encoderLeftPos);
  }
  */
  switch (state) {
    
    case 0 : // START BUTTON NOT YET PRESSED
      LCD.clear();
      LCD.print("Press Start");
      if (startbutton()) {
        updateState();
      }
      break;
//    case 23 :
//      LCD.clear(); 
//      LCD.println(analogRead(leftMostQRD));
//      LCD.print(analogRead(rightMostQRD));
//      state = 23;
//      break;

    case 1 : // STARTING STATE UNTIL FIRST GAP
      LCD.clear();
      LCD.println("Moving");
      
      //LCD.print(" L ");
      //LCD.print(lPos);
      //LCD.print("R ");
      //LCD.print(rPos);
      
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);
      
      if (stopbutton()) {
        updateState();
        
       //hanFlyo.dropBridge1(dropTheBridgePin); // dropping the first bridge will include backing up to the right distance
        //state = 0;//experiment
      }
      break;
    case 2 :
      motor.speed(rightMotor, -255);
      motor.speed(leftMotor, 255);
      motor.stop(rightMotor);
      motor.stop(leftMotor);
      motor.stop_all();
      LCD.clear();
      LCD.print("Pick Up Stuffy");
      break;
  }
}

void updateState() {
  digitalWrite(toChewPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(toChewPin, LOW);
  if (state == 0){
    state = 1;
  }else{
    state = 0;
  }
}

// INTERRUPT FUNCTIONS
void changeState() {
    if (state == 2) {
    state = 1;
    //    state = rememberState;
  } else {
    //    rememberState = state;
    state = 2;
  }
}

void getEncoderLeftPosHere() {
  //if (millis() - lastTimeLeftEnc <10) return encoderLeftPos;
  for(int i = 0; i < 2000; i++){
      if(digitalRead(leftEncoderPinA) == LOW){
        return;
      }
  }
  int B = digitalRead(leftEncoderPinB);
/*  for(int i = 0; i < 2000; i++){
      if(digitalRead(leftEncoderPinB) != B){
        return;
      }
  }
  
  if (B == LOW) {
      hanMovo.encoderLeftPos--;
    } else {
      hanMovo.encoderLeftPos++;
    }
*/  hanMovo.encoderLeftPos++;
  //  LCD.clear();
  //  LCD.print(" L "); 
 //   LCD.print(hanMovo.encoderLeftPos);
}

void getEncoderRightPosHere() {
  //if (millis() - lastTimeRightEnc < 10) return encoderRightPos;
  for(int i; i < 5000; i++){
      if(digitalRead(rightEncoderPinA) == LOW){
        return;
      }
  }
  if (digitalRead(rightEncoderPinB) == LOW) {
      hanMovo.encoderRightPos--;
    } else {
      hanMovo.encoderRightPos++;
    }
  LCD.clear();
  LCD.print(" R ");
  LCD.print(hanMovo.encoderRightPos);
}
