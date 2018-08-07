
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

// OTHER VARIABLES
long moveTime, startTime, backupCliffTime, backupTime;
double backupPercentage;

byte baseSpeed; //proportionalGain, derivativeGain;
double powerMult;
//int onTape, overCliff, backupTime;

//  HELPER FUNCTIONS
void saveMenuValues();
void raiseBasket();
void lowerBasket();
void displayQRDVals();

void setup() {
  LCD.begin();
  LCD.clear();
  RCServo0.write(0);
  pinMode(fromChewPin, INPUT);
  pinMode(irSignalPin, INPUT);
  pinMode(toChewPinLeft, OUTPUT);
  pinMode(toChewPinRight, OUTPUT);
}


void loop() {

beforeStart:
  while (!stopbutton()) {
    displayQRDVals();
  }
  delay(500);
  while (!startbutton()) {
    Menu::eePromMenu();
  }
  delay(1000);
  LCD.clear();
  saveMenuValues();
  Motion hanMovo(0);
  Crossing hanFlyo(0);


firstEwok:
  digitalWrite(toChewPinRight, HIGH);
  digitalWrite(toChewPinLeft, LOW);
  // this tells the arduino to only look for ewoks on the right side

  while (!hanFlyo.cliff()) {
    hanMovo.followTape();

    if (digitalRead(fromChewPin) == HIGH) {
      hanMovo.stopMotors();
      while (digitalRead(fromChewPin) == HIGH);
      hanMovo.findTape(findTapeWaitTime);
    }
  }
  hanMovo.stopMotors();


firstBridge:
  hanFlyo.dropBridge1();

  hanMovo.driveMotors(powerMult, powerMult);
  while (!hanFlyo.cliff()); // "cliff" signals end of the bridge
  delay(1000);


toTheIR:
  hanMovo.findTape(findTapeWaitTime);
  hanMovo.reset(-1);

  while (digitalRead(fromChewPin) == LOW) {
    hanMovo.followTape();
  }
  hanMovo.stopMotors();
  while (digitalRead(fromChewPin) == HIGH);

  // tells the arduino not to look for any stuffies
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);

  while (!hanFlyo.detect10KIR());
// might add something for realignment

stormtrooperRoom:
  //  digitalWrite(toChewPinRight, LOW);
  //  digitalWrite(toChewPinLeft, LOW);

  hanMovo.findTape(findTapeWaitTime);
  hanMovo.reset(1); // usually too far right
  {
    unsigned long startTime = millis();
    while (millis() - startTime < 2500) {
      hanMovo.followTape();
    }
  }

  // now that we are past the stormtroopers we look for a stuffy on the left
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, HIGH);

//  while (digitalRead(fromChewPin) == LOW && !hanFlyo.cliff()) {
//    hanMovo.followTape();
//  }
//  if (digitalRead(fromChewPin) == HIGH) {
//    hanMovo.stopMotors();
//    while (digitalRead(fromChewPin) == HIGH);
//    digitalWrite(toChewPinLeft, LOW);
//  }
//  while (!hanFlyo.cliff()) {
//    hanMovo.followTape();
//  }
  while (!hanFlyo.cliff()) {
    hanMovo.followTape();

    if (digitalRead(fromChewPin) == HIGH) {
      hanMovo.stopMotors();
      while (digitalRead(fromChewPin) == HIGH);
    }
  }
//  hanMovo.stopMotors();
goto beforeStart;

secondBridge:
  hanMovo.driveMotors(-powerMult, -powerMult);
  delay(backupCliffTime);
  //hanMovo.turnRight();
  hanMovo.driveMotors(powerMult, powerMult);
  while (!hanFlyo.cliff());

  hanMovo.stopMotors();
  hanMovo.driveMotors(-0.75, -0.75);
  delay(backupTime);
  motor.stop(rightMotor);
  delay(1);
  motor.stop(leftMotor);
  hanFlyo.dropBridge2(400);
  RCServo0.detach();
  //// need to drop it and then back up
  hanMovo.driveMotors(1, 1);
  while (!hanFlyo.cliff()); // "cliff" signals end of the bridge
  delay(1000);

  hanMovo.stopMotors();
  hanMovo.reset(0);


firstTower:
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, HIGH);
  //assuming we can sense this ewok on our side
  hanMovo.findRightEdge(0.45, 0.9, 1000);
  while (digitalRead(fromChewPin) == LOW) {
    hanMovo.followRightEdge();
  }
  //
  hanMovo.stopMotors();
  while (digitalRead(fromChewPin) == HIGH);

secondTower:
  //assuming we can sense chewy on our side
  while (digitalRead(fromChewPin) == LOW) {
    hanMovo.followRightEdge();
  }

  hanMovo.stopMotors();
  while (digitalRead(fromChewPin) == HIGH);

  raiseBasket();
  hanMovo.findRightEdge(0.45, 0.9, 2000);
  while (digitalRead(basketSensorPin) == HIGH) {
    hanMovo.followRightEdge();
  }
  lowerBasket();

stopSequence:
  {
    unsigned long startTime = millis();
    while (millis() - startTime < 3000) {
      hanMovo.followRightEdge();
    }
  }
  hanMovo.stopMotors();


}

void saveMenuValues() {
  baseSpeed = EEPROM[0];
  powerMult = EEPROM[1] / 100.0;
  backupTime = EEPROM[6] * 10;
  backupPercentage = EEPROM[8] / 100.0;
  backupCliffTime = EEPROM[10] * 10;
}

void raiseBasket() {
  motor.speed(scissorLiftMotor, -255);
  while (digitalRead(scissorUpLimitPin));
  motor.speed(scissorLiftMotor, 255);
  motor.stop(scissorLiftMotor);
}

void lowerBasket() {
  motor.speed(scissorLiftMotor, 255);
  while (digitalRead(scissorDownLimitPin) == HIGH);
  motor.speed(scissorLiftMotor, -255);
  motor.stop(scissorLiftMotor);
}

void displayQRDVals() {
  LCD.clear();
  LCD.print(analogRead(leftMostQRD));
  LCD.print(" ");
  LCD.print(analogRead(leftMiddleQRD));
  LCD.print(" ");
  LCD.print(analogRead(rightMiddleQRD));
  LCD.print(" ");
  LCD.print(analogRead(rightMostQRD));
  delay(1);
}


