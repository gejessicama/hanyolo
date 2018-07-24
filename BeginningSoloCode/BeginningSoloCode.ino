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

#define fromChewPin 3
#define toChewPin 8
#define dropTheBridgePin 9


#define baseSpeed 255
#define powerMultiplier 0.6
#define onTheTape 400
#define overTheCliff 750

#define pGainConst 54
#define dGainConst 0

#define distanceToStormtroopers 36
#define backUpBridgeDistance 2

volatile uint8_t state = 0;
volatile uint8_t rememberState;
volatile uint16_t rightWheelDist, leftWheelDist;

Motion hanMovo(rightMotor, leftMotor, onTheTape, overTheCliff, baseSpeed, powerMultiplier);
Crossing hanFlyo(rightMotor, leftMotor, rightMostQRD, leftMostQRD, overTheCliff, backUpBridgeDistance);

//  HELPER FUNCTIONS
void updateChewState();
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
  LCD.print("Hello");
  pinMode(fromChewPin, INPUT);
  pinMode(toChewPin, OUTPUT);
  attachInterrupt(fromChewPin, changeState, CHANGE);
}

void loop() {

  switch (state) {

    case 0 : // START BUTTON NOT YET PRESSED
      LCD.clear();
      LCD.print("Waiting");
      if (startbutton()) {
        state = 1;
        updateState();
      }
      break;
    case 23 :
      LCD.clear();
      LCD.println(analogRead(leftMostQRD));
      LCD.print(analogRead(rightMostQRD));
      state = 23;
    
    case 1 : // STARTING STATE UNTIL FIRST GAP :: could also read in QRDs to detect cliffs on the side of the robot
      LCD.clear();
      LCD.print("Moving");
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);
      if (hanFlyo.cliff()) {
        //LCD.clear();
        //LCD.print("CLiff");
        state = 23;//exp
        
       //hanFlyo.dropBridge1(dropTheBridgePin); // dropping the first bridge will include backing up to the right distance
        //state = 0;//experiment
        //updateState();
      }
      break;
    case 2 :
      //LCD.clear();
      break;
  }
}

void updateState() {
  digitalWrite(toChewPin, HIGH);
  digitalWrite(toChewPin, LOW);
}

// INTERRUPT FUNCTIONS
void changeState() {
//  LCD.clear();
//  LCD.print("Stuffy Seen");
    if (state == 2) {
    state = 1;
    //    state = rememberState;
  } else {
    //    rememberState = state;
    state = 2;
  }
}
