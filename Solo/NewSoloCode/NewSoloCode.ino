
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
uint8_t stuffyDelay;
uint16_t firstBackupTime, turningTime;
double regularPowerMult, slowPowerMult, backupPowerMult, rightWheelPercent, rampPowerMult;

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
  pinMode(fromChewPin, INPUT);
  pinMode(irSignalPin, INPUT);
  pinMode(toChewPinLeft, OUTPUT);
  pinMode(toChewPinRight, OUTPUT);
}


void loop() {

beforeStart:
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);
  RCServo0.write(0);

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

  while (!hanFlyo.cliff()) {
    hanMovo.followTape(rampPowerMult);

    if (digitalRead(fromChewPin) == HIGH) {
      delay(stuffyDelay);
      motor.stop_all();
      while (digitalRead(fromChewPin) == HIGH);
      hanMovo.driveMotors(slowPowerMult, slowPowerMult);
      delay(100);
      hanMovo.findTapeLeft(ewokFindTapeTime);
      break;
    }
  }

  while (!hanFlyo.cliff()) {
    hanMovo.followTape(slowPowerMult);

    if (digitalRead(fromChewPin) == HIGH) {
      delay(stuffyDelay);
      motor.stop_all();
      while (digitalRead(fromChewPin) == HIGH);

      hanMovo.driveMotors(slowPowerMult, slowPowerMult);
      delay(100);
      hanMovo.findTapeLeft(ewokFindTapeTime);
    }
  }
  hanMovo.stopMotors();


firstBridge:
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);

  hanMovo.driveMotors(-backupPowerMult * rightWheelPercent, -backupPowerMult);
  delay(firstBackupTime);
  motor.stop_all();

  hanFlyo.dropBridge1();
  hanMovo.driveMotors(regularPowerMult, regularPowerMult);
  while (!hanFlyo.cliff()); // "cliff" signals end of the bridge
  delay(800);


toTheIR:
  digitalWrite(toChewPinRight, HIGH);
  digitalWrite(toChewPinLeft, LOW);
  hanMovo.findTapeRight(findTapeWaitTime);
  //hanMovo.findTapeRight(findTapeWaitTime * 2);
  //hanMovo.reset(-1);

  while (digitalRead(fromChewPin) == LOW) {
    hanMovo.followTape(regularPowerMult);
  }
  delay(stuffyDelay);
  hanMovo.stopMotors();
  while (digitalRead(fromChewPin) == HIGH);

  // tells the arduino not to look for any stuffies
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);

  if (hanFlyo.detect10KIR()) {
    while (hanFlyo.detect10KIR());
  }
  while (!hanFlyo.detect10KIR());
  // might add something for realignment

stormtrooperRoom:
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);

  hanMovo.driveMotors(slowPowerMult, 0);
  delay(100);
  hanMovo.findTapeLeft(findTapeWaitTime);
  hanMovo.reset(-1);
  
  {
    unsigned long startTime = millis();
    while (millis() - startTime < 2500) {
      hanMovo.followTape(regularPowerMult);
    }
  }

secondCliff:
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, HIGH);

  while (!hanFlyo.cliff()) {
    hanMovo.followTape(regularPowerMult);
    if (digitalRead(fromChewPin) == HIGH) {
      delay(stuffyDelay);
      motor.stop_all();
      while (digitalRead(fromChewPin) == HIGH);
      hanMovo.findTapeRight(ewokFindTapeTime);
    }

  }
  hanMovo.stopMotors();
  goto beforeStart;

  
//  digitalWrite(toChewPinRight, LOW);
//  digitalWrite(toChewPinLeft, LOW);
//  hanMovo.driveMotors(-backupPowerMult, -backupPowerMult);
//  delay(100);
//
//  hanMovo.driveMotors(-slowPowerMult, -slowPowerMult);

  //returnSequence:
  // turn around and raise the basket


}

void saveMenuValues() {
  regularPowerMult = EEPROM[2] / 100.0;
  slowPowerMult = EEPROM[3] / 100.0;
  backupPowerMult = EEPROM[4] / 100.0;
  rightWheelPercent = EEPROM[5] / 100.0;
  firstBackupTime = EEPROM[6] * 10;
  stuffyDelay = EEPROM[7];
  rampPowerMult = EEPROM[8] / 100.0;
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


