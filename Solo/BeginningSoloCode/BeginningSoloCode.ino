
/*
   Code for the TINAH (Solo) that will only include up to the third stuffy, but can be added to as we gain more functionality
*/

#include <phys253.h>
#include <EEPROM.h>
#include "Motion.h"
#include "Crossing.h"
#include "Constants.h"
#include "Menu.h"

const int bridgeDropWaitTime = 2000;
const uint8_t firstBridgeServoAngle = 90;
const uint8_t secondBridgeServoAngle = 180;

// OTHER VARIABLES
uint8_t state = 0;
long moveTime, startTime;
double backupPercentage;

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
  //Serial.begin(9600);
  LCD.begin();
  LCD.clear();
  RCServo0.write(0);
  pinMode(fromChewPin, INPUT);
  pinMode(irSignalPin, INPUT);
  pinMode(toChewPinLeft, OUTPUT);
  pinMode(toChewPinRight, OUTPUT);
}


void loop() {
  //
  //  Serial.print("lout ");
  //  Serial.print(analogRead(leftMostQRD));
  //  Serial.print(" Rout ");
  //  Serial.print(analogRead(rightMostQRD));
  //  Serial.print(" lmid ");
  //  Serial.print(analogRead(leftMiddleQRD));
  //  Serial.print(" Rmid ");
  //  Serial.println(analogRead(rightMiddleQRD));
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
        //hanMovo.stopMotors();
        hanFlyo.dropBridge1(bridgeDropWaitTime, firstBridgeServoAngle, backupPercentage);
        state = 2;
      }
      break;

    case 2 : {
        // We don't want to follow tape on the bridge because that throws us off. Instead we drive staight over the bridge
        hanMovo.driveMotors();
        while (!hanFlyo.cliff());
        delay(450);
        // After the end of the bridge, we need to drive a little farther and then perform a sweep to find the tape
        while (!hanMovo.findTape(rightMiddleQRD, leftMiddleQRD, 1700));
        hanMovo.reset(-1);
        while (digitalRead(fromChewPin) == LOW) {
          hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
        }

        while (digitalRead(fromChewPin) == HIGH) {
          hanMovo.stopMotors();
          LCD.clear();
          LCD.print("Pick up Stuffy");
        }

        // after we see the stuffy, we need to tell the claw not to pick anything up and wait for the signal change
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
        digitalWrite(toChewPinRight, LOW);
        digitalWrite(toChewPinLeft, LOW);

        long st = millis();
        while (millis() - st < 2500) {
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
        //hanMovo.stopMotors();
        state = 4;
        break;
      }

    case 4 : {
        digitalWrite(toChewPinRight, LOW);
        digitalWrite(toChewPinLeft, LOW);
        hanFlyo.backUp(1.0);
        hanMovo.turnRight();

        state = 5;
        break;
      }

    case 5 : {
        hanMovo.driveMotors();
        if (hanFlyo.cliff()) {
          //hanMovo.stopMotors();
          hanFlyo.dropBridge2(bridgeDropWaitTime, secondBridgeServoAngle, 1.0);
          digitalWrite(toChewPinRight, HIGH);
          digitalWrite(toChewPinLeft, LOW);
          state = 6;
        }
      }
    case 6 : {
        hanMovo.driveMotors();
        while (!hanFlyo.cliff());
        delay(450);
        while (digitalRead(fromChewPin) == HIGH) {
          hanMovo.stopMotors();
          LCD.clear();
          LCD.print("Pick up Stuffy");
        }
        state = 7;
      }
    case 7 : {
        hanMovo.followRightEdge(rightOutQRD, rightInQRD);
        while (digitalRead(fromChewPin) == HIGH) {
          hanMovo.stopMotors();
          LCD.clear();
          LCD.print("Pick up Stuffy");
        }
      }
  }
}

void saveMenuValues() {
  backupPercentage = EEPROM[8] / 100.0;
}



