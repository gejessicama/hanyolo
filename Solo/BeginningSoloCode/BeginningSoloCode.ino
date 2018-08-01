
/*
   Code for the TINAH (Solo) that will only include up to the third stuffy, but can be added to as we gain more functionality

   Certian Values are stored in EEPROM, and there is a menu system for reading these in:
*/

#include <phys253.h>
#include <EEPROM.h>
#include "Motion.h"
#include "Crossing.h"
#include "Constants.h"
#include "Menu.h"

const int bridgeDropWaitTime = 1000;
const uint8_t firstBridgeServoAngle = 110;
const double rightWheelPercentage1 = 0.6;

// OTHER VARIABLES
uint8_t state = 0;
long moveTime;

// EEPROM VARIABLES
long timeToIR;
//byte baseSpeed;
//double powerMult;

//, proportionalGain, derivativeGain;
//int onTape, overCliff, backupTime;

Motion hanMovo(rightMotor, leftMotor);
Crossing hanFlyo(rightMotor, leftMotor, rightMostQRD, leftMostQRD, irSignalPin);

//  HELPER FUNCTIONS
void saveMenuValues();
void raiseBasket();
void lowerBasket();

void setup() {
  LCD.begin();
  LCD.clear();
  pinMode(fromChewPin, INPUT);
  pinMode(irSignalPin, INPUT);
  pinMode(toChewPin, OUTPUT);
}


void loop() {
  switch (state) {

    case 0 : // START BUTTON NOT YET PRESSED: calls the menu until we press start
      while (!startbutton()) {
        Menu::eePromMenu();
      }

      delay(1000);
      saveMenuValues();
      hanMovo.setConstants();
      hanFlyo.setConstants();
      state = 1;
      break;

    case 1 : // STARTING STATE UNTIL FIRST GAP: follows tape and responds to signal from Arduino
      LCD.clear();
      LCD.print("Move");
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);

      while (digitalRead(fromChewPin) == HIGH) {
        hanMovo.stopMotors();
        LCD.clear();
        LCD.print("Pick up Stuffy");
      }

      if (hanFlyo.cliff()) {
        hanMovo.stopMotors();
        hanFlyo.dropBridge(bridgeDropWaitTime, firstBridgeServoAngle, rightWheelPercentage1);
        hanMovo.driveMotors();
          
         // delay(250);
        state = 2;
        moveTime = millis() + timeToIR;
        // takes the current time and adds the amount of time until we should be in front of the IR signal
      }
      break;

    case 2 :
      long st = millis();
      long et = st;
      while (et - st < 1250.0) {
        hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
        et = millis();
      }
      motor.speed(rightMotor, -255);
      motor.speed(leftMotor, 255);
      motor.stop_all();
      while (!hanFlyo.detect10KIR()) {
        LCD.print("not 10k");
        LCD.clear();
      }
      LCD.clear();
      LCD.print("10k");
      delay(1000);
      state = 4;
      break;

//    case 2 :
//      LCD.clear();
//      LCD.print("ToSignal");
//      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
//
//      while (digitalRead(fromChewPin) == HIGH) {
//        motor.speed(rightMotor, -255);
//        motor.speed(leftMotor, 255);
//        motor.stop_all();
//        LCD.clear();
//        LCD.print("Pick up Stuffy");
//      }
//
//      if (millis() > moveTime) {
//        motor.speed(rightMotor, -255);
//        motor.speed(leftMotor, 255);
//        motor.stop_all();
//        //next state??? Where we wait for the IR signal
//      }
//
//
//    case 3:
//      if (hanFlyo.detect10KIR()){
//        //update state here??
//      }
//
//      while (!hanFlyo.detect10KIR()) {
//        LCD.print("not 10k");
//        LCD.clear();
//      }
//      LCD.clear();
//      LCD.print("10k");
//      delay(1000);
//      state = 4;
//      break;

      //    case 4 :
      //      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
      //      if (hanFlyo.cliff());
      //      break;



  }
}

void saveMenuValues() {
  timeToIR = EEPROM[8] * 20;
}



