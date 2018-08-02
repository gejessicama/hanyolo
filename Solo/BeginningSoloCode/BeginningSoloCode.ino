
/*
   Code for the TINAH (Solo) that will only include up to the third stuffy, but can be added to as we gain more functionality
*/

#include <phys253.h>
#include <EEPROM.h>
#include "Motion.h"
#include "Crossing.h"
#include "Constants.h"
#include "Menu.h"

const int bridgeDropWaitTime = 1000;
const uint8_t firstBridgeServoAngle = 90;
const uint8_t secondBridgeServoAngle = 180;

// OTHER VARIABLES
uint8_t state = 0;
long moveTime, timeToIR, startTime;

//byte baseSpeed, proportionalGain, derivativeGain;
//double powerMult;
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
  pinMode(toChewPinLeft, OUTPUT);
  pinMode(toChewPinRight, OUTPUT);
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
      digitalWrite(toChewPinRight, HIGH);
      digitalWrite(toChewPinLeft, LOW);
      state = 1;
      break;

    case 1 :  // STARTING STATE UNTIL FIRST GAP: follows tape and responds to signal from Arduino
      LCD.clear();
      LCD.print("Move");
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);

      while (digitalRead(fromChewPin) == HIGH) {

        hanMovo.stopMotors();
        LCD.clear();
        LCD.print("Pick up Stuffy");
      }

      if (hanFlyo.cliff()) {
        hanFlyo.dropBridge1(bridgeDropWaitTime, firstBridgeServoAngle, 0.6);
        state = 2;
        //moveTime = millis() + timeToIR;
        // takes the current time and adds the amount of time until we should be in front of the IR signal
      }
      break;

    case 2 : {
        long str0 = millis();
        while (millis() - str0 < 500) {
          hanMovo.driveMotors();
        }
        //        long str = millis();
        //
        //        while (millis() - str < timeToIR) {
        //          hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
        //
        //          while (digitalRead(fromChewPin) == HIGH) {
        //            hanMovo.stopMotors();
        //            LCD.clear();
        //            LCD.print("Pick up Stuffy");
        //          }
        //        }
        while (digitalRead(fromChewPin) == LOW) {
          hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
        }
        while (digitalRead(fromChewPin) == HIGH) {
          hanMovo.stopMotors();
          LCD.clear();
          LCD.print("Pick up Stuffy");
        }

        hanMovo.stopMotors();
        digitalWrite(toChewPinRight, LOW);
        digitalWrite(toChewPinLeft, LOW);

        while (!hanFlyo.detect10KIR()) {
          LCD.print("not 10k");
          LCD.clear();
        }
        LCD.clear();
        LCD.print("10k");
        state = 3;
        break;
      }


    case 3 : {
        long st = millis();
        digitalWrite(toChewPinRight, LOW);
        digitalWrite(toChewPinLeft, LOW);
        while (millis() - st < 4000) {
          hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
        }
        digitalWrite(toChewPinRight, LOW);
        digitalWrite(toChewPinLeft, HIGH);

        while (!hanFlyo.cliff()) {

          hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
          while (digitalRead(fromChewPin) == HIGH) {
            hanMovo.stopMotors();
            LCD.clear();
            LCD.print("Pick up Stuffy");
          }
        }
        state = 4;
        break;
      }

    case 4 : {
        digitalWrite(toChewPinRight, LOW);
        digitalWrite(toChewPinLeft, LOW);
        hanFlyo.backUp(1.0);
        hanMovo.turnRight();
        //hanFlyo.alignStep();
        state = 5;
        break;
      }
    case 5 : {
        
        hanMovo.driveMotors();
        if (hanFlyo.cliff()){
          hanFlyo.dropBridge2(bridgeDropWaitTime,secondBridgeServoAngle,1.0);
        }
      }
  }
}

void saveMenuValues() {
  timeToIR = EEPROM[8] * 20;
}



