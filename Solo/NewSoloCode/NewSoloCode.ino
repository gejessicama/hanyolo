
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
  //  LCD.begin();
  //  LCD.clear();
  RCServo0.write(0);
  pinMode(fromChewPin, INPUT);
  pinMode(irSignalPin, INPUT);
  pinMode(toChewPinLeft, OUTPUT);
  pinMode(toChewPinRight, OUTPUT);
}


void loop() {

beforeStart:
  while (!startbutton()) {
    Menu::eePromMenu();
  }
  delay(1000);
  saveMenuValues();
  hanMovo.setConstants();
  hanFlyo.setConstants();


firstEwok:
  digitalWrite(toChewPinRight, HIGH);
  digitalWrite(toChewPinLeft, LOW);
  // this tells the arduino to only look for ewoks on the right side

  while (!hanFlyo.cliff()) {
    hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);

    if (digitalRead(fromChewPin) == HIGH) {
      hanMovo.stopMotors();
      while (digitalRead(fromChewPin) == HIGH);
    }
  }
  hanMovo.stopMotors();


firstBridge:
  hanFlyo.dropBridge1(bridgeDropWaitTime, firstBridgeServoAngle, backupPercentage);
  hanMovo.driveMotors();
  while (!hanFlyo.cliff()); // "cliff" signals end of the bridge
  delay(400);


toTheIR:
  hanMovo.findTape(rightMiddleQRD, leftMiddleQRD, 1700);
  hanMovo.reset(-1);

  while (digitalRead(fromChewPin) == LOW) {
    hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
  }
  hanMovo.stopMotors();
  while (digitalRead(fromChewPin) == HIGH);

  // tells the arduino not to look for any stuffies
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);

  while (!hanFlyo.detect10KIR());


stormtrooperRoom:
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);

  {
    unsigned long startTime = millis();
    while (millis() - startTime < 2500) {
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
    }
  }

  // now that we are past the stormtroopers we look for a stuffy on the left
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, HIGH);

  while (!hanFlyo.cliff()) {
    hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);

    if (digitalRead(fromChewPin) == HIGH) {
      hanMovo.stopMotors();
      while (digitalRead(fromChewPin) == HIGH);
    }
  }
  hanMovo.stopMotors();


secondBridge:
  hanFlyo.backUp(2);
  hanFlyo.backUp(1);

  hanMovo.driveMotors();
  while (!hanFlyo.cliff());

  hanMovo.stopMotors();
  hanFlyo.dropBridge2(bridgeDropWaitTime, secondBridgeServoAngle, 1);
  hanMovo.driveMotors();
  while (!hanFlyo.cliff()); // "cliff" signals end of the bridge
  delay(400);


firstTower:
  //assuming we can sense this ewok on our side
  while (digitalRead(fromChewPin) == LOW) {
    hanMovo.followRightEdge(rightOutQRD, rightInQRD);
  }

  hanMovo.stopMotors();
  while (digitalRead(fromChewPin) == HIGH);

secondTower:
  //assuming we can sense chewy on our side
  while (digitalRead(fromChewPin) == LOW) {
    hanMovo.followRightEdge(rightOutQRD, rightInQRD);
  }

  hanMovo.stopMotors();
  while (digitalRead(fromChewPin) == HIGH);

  raiseBasket();
  while (digitalRead(basketSensorPin) == HIGH) {
    hanMovo.followRightEdge(rightOutQRD, rightInQRD);
  }
  lowerBasket();

stopSequence:
  {
    unsigned long startTime = millis();
    while (millis() - startTime < 3000) {
      hanMovo.followRightEdge(rightOutQRD, rightInQRD);
    }
  }
  hanMovo.stopMotors();

}

void saveMenuValues() {
  backupPercentage = EEPROM[8] / 100.0;
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



