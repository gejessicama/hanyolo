
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
uint16_t firstBackupTime, secondBackupTime, turningTime;
double regularPowerMult, slowPowerMult, backupPowerMult, rightWheelPercent;

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
  goto firstEwok;

firstEwok:
  digitalWrite(toChewPinRight, HIGH);
  digitalWrite(toChewPinLeft, LOW);
  // this tells the arduino to only look for ewoks on the right side

  while (!hanFlyo.cliff()) {
    hanMovo.followTape();

    if (digitalRead(fromChewPin) == HIGH) {
      delay(10);
      motor.stop_all();
      while (digitalRead(fromChewPin) == HIGH);

      hanMovo.findTapeLeft(findTapeWaitTime); // change this thing for different times
    }
  }
  hanMovo.stopMotors();


firstBridge:
  hanMovo.driveMotors(-backupPowerMult * rightWheelPercent, -backupPowerMult);
  delay(firstBackupTime);
  motor.stop_all();
  hanFlyo.dropBridge1();
  hanMovo.driveMotors(regularPowerMult, regularPowerMult);
  while (!hanFlyo.cliff()); // "cliff" signals end of the bridge
  delay(1000);


toTheIR:
  hanMovo.findTapeRight(findTapeWaitTime);
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
  hanMovo.driveMotors(slowPowerMult, 0);
  delay(100);
  hanMovo.findTapeLeft(findTapeWaitTime);
  hanMovo.reset(-1);

  while (!hanFlyo.cliff()) {
    hanMovo.followTape();
  }
  hanMovo.stopMotors();

  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, HIGH);
  hanMovo.driveMotors(-backupPowerMult, -backupPowerMult);
  delay(100);
  hanMovo.driveMotors(-slowPowerMult, -slowPowerMult);

  {
    unsigned long startTime = millis();
    while ((digitalRead(fromChewPin) == LOW) && (millis() - startTime < 1500));
  }

  motor.stop_all();
  while (digitalRead(fromChewPin) == HIGH);
  hanMovo.findTapeRight(findTapeWaitTime);

  hanMovo.stopMotors();
  goto beforeStart;


  // now that we are past the stormtroopers we look for a stuffy on the left
//  digitalWrite(toChewPinRight, LOW);
//  digitalWrite(toChewPinLeft, HIGH);
//
//  while (!hanFlyo.cliff()) {
//    hanMovo.followTape();
//
//    if (digitalRead(fromChewPin) == HIGH) {
//      //hanMovo.stopMotors();
//      delay(10);
//      motor.stop_all();
//      while (digitalRead(fromChewPin) == HIGH);
//      hanMovo.findTapeRight(findTapeWaitTime);
//    }
//  }
//  hanMovo.stopMotors();
//  goto beforeStart;

secondBridge:
  //  hanMovo.driveMotors(-backupPowerMult, -backupPowerMult);
  //  delay(firstBackupTime);
  //  hanMovo.driveMotors(-slowPowerMult, -slowPowerMult);
  //  delay(secondBackupTime);
  //  motor.stop_all();
  //  delay(10);
  hanMovo.driveMotors(-slowPowerMult * 1.55, -slowPowerMult * .6); // pretty sketchy rn tbh
  delay(turningTime);
  motor.stop_all();
  delay(10);
  hanMovo.driveMotors(slowPowerMult, slowPowerMult);
  delay(800);
  while (!hanFlyo.cliff());

  hanMovo.stopMotors();
  goto beforeStart;

  hanMovo.driveMotors(-0.75, -0.75);
  //delay(backupTime);
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
  regularPowerMult = EEPROM[2] / 100.0;
  slowPowerMult = EEPROM[3] / 100.0;
  backupPowerMult = EEPROM[4] / 100.0;
  rightWheelPercent = EEPROM[5] / 100.0;
  firstBackupTime = EEPROM[6] * 10;
  secondBackupTime = EEPROM[7] * 10;
  turningTime = EEPROM[8] * 10;
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


