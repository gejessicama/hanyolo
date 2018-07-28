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

#define basketSensorPin 7
#define scissorUpLimitPin 6
#define scissorDownLimitPin 5
//#define oneIRPin
//#define tenIRPin



#define fromChewPin 3
#define toChewPin 8
#define dropTheBridgePin 9



#define bSpeed 200
#define powerMult 0.4

#define onTheTape 400
#define overTheCliff 650

uint8_t pGainConst =  54;
uint8_t dGainConst  = 0;

#define distanceToStormtroopers 36
#define backUpBridgeDistance 2

volatile uint8_t state = 0;
volatile uint8_t rememberState;
volatile uint16_t rightWheelDist, leftWheelDist;

Motion hanMovo(rightMotor, leftMotor, onTheTape, overTheCliff, bSpeed, powerMult);
Crossing hanFlyo(rightMotor, leftMotor, rightMostQRD, leftMostQRD, overTheCliff, backUpBridgeDistance);

//  HELPER FUNCTIONS
void updateState();
void raiseBasket();
void lowerBasket();

//  INTERRUPT FUNCTIONS
void changeState();

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


int setupStage = 0;
int vel = 100;
int bt = 0; // 390-410



void loop() {

  switch (state) {

    case 0 : // START BUTTON NOT YET PRESSED
      LCD.clear();
      LCD.print("Start| Stop>nx");
      LCD.println(setupStage);
      if (stopbutton()) {
        delay(200);
        setupStage++;
        //updateSetup();
      }
      if (setupStage == 0) {
        hanMovo.powerMultiplier =  knob(6) / 1024.0;
        hanMovo.baseSpeed = knob(7) / 1024.0 * 255;
        LCD.print("Pow ");
        LCD.print(hanMovo.powerMultiplier);
        LCD.print(" Vel ");
        LCD.print(hanMovo.baseSpeed);
      }
      if (setupStage == 1) {
        pGainConst = knob(6) / 1024.0 * 200;
        dGainConst = knob(7) / 1024.0 * 200;
        LCD.print("kp");
        LCD.print(pGainConst);
        LCD.print("kd");
        LCD.print(dGainConst);
      }
      if (setupStage == 2) {
        hanMovo.ON =  knob(6);
        hanMovo.CLIFF = knob(7);
        LCD.print("ON ");
        LCD.print(hanMovo.ON);
        LCD.print(" CL ");
        LCD.print(hanMovo.CLIFF);
      }
      if (setupStage == 3) {
        vel = 255.0 * knob(7) / 1024.0;
        bt = 10 * knob(6);/*
        LCD.print("vel");
        LCD.print(vel);*/
        LCD.print(" bt ");
        LCD.print(bt);
      }
      if (startbutton()) {
        state++;
      }
      break;

    case 1 : // drive to second gap
      raiseBasket();
      delay(3000);
      lowerBasket();
//      hanMovo.driveMotors(bSpeed);
//
//      if (hanFlyo.cliff()) {
//        long st = millis();
//        long ct = millis();
//        while (ct - st < bt) {
//          hanMovo.driveMotors(-255);
//          ct = millis();
//        }
//        hanMovo.driveMotors(0);
//        delay(500);
//        RCServo0.write(180);
//        delay(500);
//        hanMovo.driveMotors(bSpeed);
//        state++;
//      }
//      break;
    case 2 : //drive up bridge
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);
      if (hanFlyo.cliff()) {
        attachInterrupt(fromChewPin, changeState, CHANGE);
        state++;
      }
    case 3 : //follow edge of tower
      hanMovo.followRightEdge(rightOutQRD, rightInQRD, pGainConst, dGainConst);
      if (digitalRead(basketSensorPin) == LOW) {
        lowerBasket();
        state ++;
      }
      if (stopbutton()) {
        state ++;
      }
      break;

    case 4 : //finished
    break;
  }
}

void raiseBasket(){
  motor.speed(scissorLiftMotor, -255);
  while(digitalRead(scissorUpLimitPin) == HIGH){
    
  }
  motor.speed(scissorLiftMotor, 255);
  motor.stop(scissorLiftMotor);
}

void lowerBasket(){
  motor.speed(scissorLiftMotor,255);
  while(digitalRead(scissorDownLimitPin) == HIGH){
    
  }
  motor.speed(scissorLiftMotor, -255);
  motor.stop(scissorLiftMotor);
}


// INTERRUPT FUNCTIONS
void changeState() {
  motor.speed(rightMotor, -255);
  motor.speed(leftMotor, 255);
  motor.stop(rightMotor);
  motor.stop(leftMotor);
  if (state == 4) {
    state = rememberState;
  } else {
    raiseBasket();
    rememberState = state;
    state = 4;
  }
}

void updateSetup() {
  if (setupStage == 0) {
    setupStage = 1;
  }
  if (setupStage == 1) {
    setupStage = 2;
  }
}
