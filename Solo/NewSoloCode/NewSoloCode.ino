
/*
   Code for the TINAH that is reformatted to hopefully be more efficient

   Potential problems: I added hanMovo.stopMotors() when we sense a cliff that was not there before
    and might cause us to have to readdjust our back up times and such

    look over stuff for the second bridge crossing: it's a bit sketchy

    all the tower stuff is purely theoretical and may not actually work

  TODO: find all raw numbers and try to figure out a better way
    run code and optimize values
*/

#include <phys253.h>
#include <EEPROM.h>
#include "Motion.h"
#include "Crossing.h"
#include "Constants.h"
#include "Menu.h"

const int bridgeDropWaitTime = 1500;
const uint8_t firstBridgeServoAngle = 90;
const uint8_t secondBridgeServoAngle = 170;
const uint16_t findTapeWaitTime = 1700;

// OTHER VARIABLES
long moveTime, startTime, backupCliffTime;
double backupPercentage;

//byte baseSpeed, proportionalGain, derivativeGain;
double powerMult;
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
  RCServo0.write(90);
  pinMode(fromChewPin, INPUT);
  pinMode(irSignalPin, INPUT);
  pinMode(toChewPinLeft, OUTPUT);
  pinMode(toChewPinRight, OUTPUT);
}


void loop() {

//beforeStart:
  while (!startbutton()) {
    Menu::eePromMenu();
  }
  delay(1000);
  saveMenuValues();
  hanMovo.setConstants();
  hanFlyo.setConstants();


//firstEwok:
  LCD.clear();

//  digitalWrite(toChewPinRight, HIGH);
//  digitalWrite(toChewPinLeft, LOW);
//  // this tells the arduino to only look for ewoks on the right side
//
//  while (!hanFlyo.cliff()) {
//    hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
//
//    if (digitalRead(fromChewPin) == HIGH) {
//      
//      hanMovo.stopMotors();
//      while (digitalRead(fromChewPin) == HIGH);
//    }
//  }
//  hanMovo.stopMotors();
//
//
////firstBridge:
//  hanFlyo.dropBridge1(bridgeDropWaitTime, firstBridgeServoAngle, backupPercentage);
//
//  hanMovo.driveMotors(powerMult);
//  while (!hanFlyo.cliff()); // "cliff" signals end of the bridge
//  delay(400);
//
//
////toTheIR:
//  hanMovo.findTape(rightMiddleQRD, leftMiddleQRD, findTapeWaitTime);
//  hanMovo.reset(-1);
//
//  while (digitalRead(fromChewPin) == LOW) {
//    hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
//  }
//  hanMovo.stopMotors();
//  while (digitalRead(fromChewPin) == HIGH);
//
//  // tells the arduino not to look for any stuffies
//  digitalWrite(toChewPinRight, LOW);
//  digitalWrite(toChewPinLeft, LOW);
//
//  while (!hanFlyo.detect10KIR());
//
//
////stormtrooperRoom:
//  digitalWrite(toChewPinRight, LOW);
//  digitalWrite(toChewPinLeft, LOW);
//
//  hanMovo.findTape(rightMiddleQRD, leftMiddleQRD, findTapeWaitTime);
//  hanMovo.reset(-1);
//  {
//    unsigned long startTime = millis();
//    while (millis() - startTime < 2500) {
//      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
//    }
//  }

  // now that we are past the stormtroopers we look for a stuffy on the left
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, HIGH);

  while (digitalRead(fromChewPin) == LOW && !hanFlyo.cliff()){
    hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
  }
  if (digitalRead(fromChewPin) == HIGH) {
      hanMovo.stopMotors();
      while (digitalRead(fromChewPin) == HIGH);
      digitalWrite(toChewPinLeft, LOW);
  }
  while(!hanFlyo.cliff()){
    hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
  }
  hanMovo.stopMotors();
  //goto beforeStart;


//secondBridge:
  hanMovo.driveMotors(-powerMult);
  delay(backupCliffTime);
  hanMovo.turnRight();
  hanMovo.driveMotors(powerMult);
  while (!hanFlyo.cliff());

  hanMovo.stopMotors();
  //hanFlyo.backUp(backupPercentage);
  hanFlyo.dropBridge2(bridgeDropWaitTime, secondBridgeServoAngle, backupPercentage);
// need to drop it and then back up
  hanMovo.driveMotors(1.0);
  while (!hanFlyo.cliff()); // "cliff" signals end of the bridge
  delay(1000);

  hanMovo.stopMotors();
//  hanMovo.reset(0);
//
//
//firstTower:
//  digitalWrite(toChewPinRight, LOW);
//  digitalWrite(toChewPinLeft, HIGH);
//  //assuming we can sense this ewok on our side
//  hanMovo.findRightEdge(rightOutQRD, rightInQRD, 1000);
//  while (digitalRead(fromChewPin) == LOW) {
//    hanMovo.followRightEdge(rightOutQRD, rightInQRD);
//  }
////
//  hanMovo.stopMotors();
//  while (digitalRead(fromChewPin) == HIGH);
//
//secondTower:
//  //assuming we can sense chewy on our side
//  while (digitalRead(fromChewPin) == LOW) {
//    hanMovo.followRightEdge(rightOutQRD, rightInQRD);
//  }
//
//  hanMovo.stopMotors();
//  while (digitalRead(fromChewPin) == HIGH);
//
//  raiseBasket();
//  while (digitalRead(basketSensorPin) == HIGH) {
//    hanMovo.followRightEdge(rightOutQRD, rightInQRD);
//  }
//  lowerBasket();
//
//stopSequence:
//  {
//    unsigned long startTime = millis();
//    while (millis() - startTime < 3000) {
//      hanMovo.followRightEdge(rightOutQRD, rightInQRD);
//    }
//  }
//  hanMovo.stopMotors();

}

void saveMenuValues() {
  powerMult = EEPROM[1] / 100.0;
  backupPercentage = EEPROM[8] / 100.0;
  backupCliffTime = EEPROM[10] * 10;
}

void raiseBasket() {
  motor.speed(scissorLiftMotor, 255);
  while(digitalRead(scissorUpLimitPin) == HIGH);
  motor.speed(scissorLiftMotor, -255);
  motor.stop(scissorLiftMotor);
}

void lowerBasket() {
  motor.speed(scissorLiftMotor, -255);
  while(digitalRead(scissorDownLimitPin) == HIGH);
  motor.speed(scissorLiftMotor, 255);
  motor.stop(scissorLiftMotor);
}



