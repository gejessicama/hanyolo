
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
uint16_t firstBackupTime, tempTime;
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
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);

  //  pinMode(fromChewPin, INPUT);
  //  pinMode(irSignalPin, INPUT);
  //  pinMode(basketSensorPin, INPUT);
  //  pinMode(scissorUpLimitPin, INPUT);
  //  pinMode(scissorDownLimitPin, INPUT);

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
  //lowerBasket();
  //goto secondCliff;


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
//  LCD.clear();
//  LCD.print("Arduino ");
  LCD.println(digitalRead(fromChewPin));


firstBridge:
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);

  hanMovo.driveMotors(-backupPowerMult * rightWheelPercent, -backupPowerMult);
  delay(firstBackupTime);
  motor.stop_all();

  hanFlyo.dropBridge1();
  hanMovo.driveMotors(regularPowerMult, regularPowerMult);
  while (!hanFlyo.cliff()); // "cliff" signals end of the bridge
//  LCD.clear();
//  LCD.print("Arduino ");
//  LCD.println(digitalRead(fromChewPin));
//  LCD.print("Bridge end");
  delay(800);
//  LCD.clear();


toTheIR:
  digitalWrite(toChewPinRight, HIGH);
  digitalWrite(toChewPinLeft, LOW);
  hanMovo.findTapeRight(findTapeWaitTime);
//  LCD.print("Arduino ");
//  LCD.println(digitalRead(fromChewPin));
  while (digitalRead(fromChewPin) == LOW) {
    hanMovo.followTape(regularPowerMult);
  }
  delay(stuffyDelay);
  hanMovo.stopMotors();
//  LCD.clear();
//  LCD.print("Arduino ");
//  LCD.println(digitalRead(fromChewPin));
//  LCD.print("Pick up 2");
  //delay(1000);
  while (digitalRead(fromChewPin) == HIGH);
  hanMovo.stopMotors();

  // could adjust here and keep checking for the stuffy

  // tells the arduino not to look for any stuffies
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);

  if (hanFlyo.detect10KIR()) {
    LCD.clear();
    LCD.print("IR 10k p1");
    while (hanFlyo.detect10KIR());
  }
  LCD.clear();
  LCD.print("IR not 10k");
  while (!hanFlyo.detect10KIR());
  LCD.clear();
  LCD.print("10K");
  //hanMovo.
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
  delay(1000);


turnAround:
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);
  hanMovo.driveMotors(-backupPowerMult, -backupPowerMult);
  delay(firstBackupTime);

  hanMovo.driveMotors(slowPowerMult, -slowPowerMult);
  delay(500);
  hanMovo.findTapeLeft(5000);

  //  ALTERNATIVE RETURN STRATEGY
  //  hanMovo.driveMotors(-slowPowerMult, slowPowerMult);
  //  delay(500);
  //  hanMovo.findTapeRight(5000);


returnSequence:

  {
    unsigned long startTime = millis();
    while (millis() - startTime < 1000) {
      hanMovo.followTape(slowPowerMult);
    }
  }
  motor.stop_all();
  raiseBasket();

  {
    unsigned long startTime = millis();
    while (millis() - startTime < tempTime) {
      hanMovo.followTape(slowPowerMult);
    }
  }

  //  ALTERNATIVE RETURN STRATEGY
  //  {
  //    unsigned long startTime = millis();
  //    while (millis() - startTime < 1000){
  //      hanMovo.driveMotors(slowPowerMult*1.25, slowPowerMult);
  //    }
  //  }
  //  motor.stop_all();
  //  raiseBasket();
  //
  //  {
  //    unsigned long startTime = millis();
  //    while (millis() - startTime < tempTime){
  //      hanMovo.driveMotors(slowPowerMult*1.25, slowPowerMult);
  //    }
  //  }

  motor.stop_all();
  lowerBasket();
  goto beforeStart;
}

void saveMenuValues() {
  regularPowerMult = EEPROM[2] / 100.0;
  slowPowerMult = EEPROM[3] / 100.0;
  backupPowerMult = EEPROM[4] / 100.0;
  rightWheelPercent = EEPROM[5] / 100.0;
  firstBackupTime = EEPROM[6] * 10;
  stuffyDelay = EEPROM[7];
  rampPowerMult = EEPROM[8] / 100.0;
  tempTime = EEPROM[9] * 20;
}

void raiseBasket() {
  motor.speed(scissorLiftMotor, -200);
  while (digitalRead(scissorUpLimitPin) == HIGH);
  motor.speed(scissorLiftMotor, 200);
  delay(10);
  motor.stop(scissorLiftMotor);
}

void lowerBasket() {
  LCD.clear();
  LCD.print("Lower Basket <Start>");
  while (!startbutton());
  delay(800);
  motor.speed(scissorLiftMotor, 200);
  while (!startbutton());
  motor.speed(scissorLiftMotor, -255);
  motor.stop(scissorLiftMotor);
  delay(800);
  //  motor.speed(scissorLiftMotor, 200);
  //  while (digitalRead(scissorDownLimitPin) == HIGH);
  //  motor.speed(scissorLiftMotor, -200);
  //  delay(10);
  //  motor.stop(scissorLiftMotor);
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


