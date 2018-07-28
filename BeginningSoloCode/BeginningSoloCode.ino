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

#define IRsig 7

#define bSpeed 255
#define powerMult 0.4

#define onTheTape 400
#define overTheCliff 650

uint8_t pGainConst =  54;
uint8_t dGainConst  = 0;

#define distanceToStormtroopers 36
#define backUpBridgeDistance 2

volatile uint8_t state = 0;
volatile uint8_t pstate = 0;
volatile uint8_t rememberState;
volatile uint16_t rightWheelDist, leftWheelDist;

Motion hanMovo(rightMotor, leftMotor, onTheTape, overTheCliff, bSpeed, powerMult, leftEncoderPinA, leftEncoderPinB, rightEncoderPinA, rightEncoderPinB);
Crossing hanFlyo(rightMotor, leftMotor, rightMostQRD, leftMostQRD, overTheCliff, backUpBridgeDistance,IRsig);

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
  RCServo0.write(0);
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
int bt = 0; // 390-410 - optimal
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
        bt = 10 * knob(6);
        /*LCD.print("vel");
        LCD.print(vel);*/
        LCD.print(" ft ");
        LCD.print(bt);
      }
      if (startbutton()) {
        updateState();
      }
      break;
    case 23 :
      LCD.clear();
      LCD.println(analogRead(leftMostQRD));
      LCD.print(analogRead(rightMostQRD));
      state = 23;
      break;

    case 1 : // STARTING STATE UNTIL FIRST GAP
      //hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);
      //hanMovo.driveMotors(vel);
      hanMovo.followRightEdge(rightOutQRD,rightInQRD,pGainConst, dGainConst);/*
      if (hanFlyo.cliff()) { // detect cliff then reverse for bt time
        hanFlyo.dropBridge1(toChewPin);
        state = 3;
      }

      */
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

    case 3 :
      long st = millis();
      long et = st;
      while (et - st < bt){
        hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);
      }
      motor.stop_all();
      while (!hanFlyo.detectIR()){
        LCD.print("1k");
      }
      LCD.print("10k");
      break;
  }
}

void updateState() {
  // Changed to Low then HIGH
  digitalWrite(toChewPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(toChewPin, LOW);
  if (state == 0) {
    state = 1;
  } else {
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





