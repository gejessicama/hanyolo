
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
uint16_t firstBackupTime, returnTime;
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
//      hanMovo.driveMotors(slowPowerMult, slowPowerMult);
//      delay(100);
//      hanMovo.findTapeLeft(ewokFindTapeTime);
      break;
    }
  }

  while (!hanFlyo.cliff()) {
    hanMovo.followTape(slowPowerMult);

    if (digitalRead(fromChewPin) == HIGH) {
      delay(stuffyDelay);
      motor.stop_all();
      while (digitalRead(fromChewPin) == HIGH);

//      hanMovo.driveMotors(slowPowerMult, slowPowerMult);
//      delay(100);
//      hanMovo.findTapeRight(ewokFindTapeTime);
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
  hanMovo.driveMotors(-slowPowerMult, -slowPowerMult);
  delay(300);
  motor.stop_all();
  delay(10);
  
  hanMovo.driveMotors(regularPowerMult, regularPowerMult);
  while (!hanFlyo.cliff());
  delay(200);
  
  hanMovo.findTapeLeft(findTapeWaitTime);
  while (!hanFlyo.cliff()) {
    hanMovo.followTape(regularPowerMult);
  }
  delay(300);


secondStuffy:
  digitalWrite(toChewPinRight, HIGH);
  digitalWrite(toChewPinLeft, LOW);
  hanMovo.findTapeRight(findTapeWaitTime);

  while (digitalRead(fromChewPin) == LOW) {
    hanMovo.followTape(regularPowerMult);
  }
  hanMovo.driveMotors(regularPowerMult, -regularPowerMult);
  delay(stuffyDelay);
  hanMovo.stopMotors();

  while (digitalRead(fromChewPin) == HIGH);


toTheIR:
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


stormtrooperRoom:
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);

  hanMovo.driveMotors(slowPowerMult, 0);
  delay(100);
  hanMovo.findTapeLeft(findTapeWaitTime);
  //hanMovo.reset(-1);

  {
    unsigned long startTime = millis();
    while (millis() - startTime < 2500) { //should be enough time to put us on the platform
      hanMovo.followTape(regularPowerMult);
    }
  }

  //  {
  //    unsigned long startTime = millis();
  //    while (millis() - startTime < 1500) {
  //      hanMovo.followTape(regularPowerMult);
  //      hanMovo.lostAndFindTape(); // if we lose the tape, we will search for it
  //    }
  //  }


secondCliff:
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, HIGH);

  while (!hanFlyo.cliff()) {
    hanMovo.followTape(slowPowerMult);

    if (digitalRead(fromChewPin) == HIGH) {
      delay(stuffyDelay);
      motor.stop_all();
      while (digitalRead(fromChewPin) == HIGH);
      hanMovo.findTapeRight(ewokFindTapeTime);
    }
  }
  hanMovo.stopMotors();
  delay(waitForClaw);


turnAround:
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);
  hanMovo.driveMotors(-backupPowerMult, -backupPowerMult);
  delay(firstBackupTime);

  hanMovo.driveMotors(slowPowerMult, -slowPowerMult);
  delay(500);
  hanMovo.findTapeLeft(5000);


//returnSequence:
//
//  {
//    unsigned long startTime = millis();
//    while (millis() - startTime < 1000) {
//      hanMovo.followTape(slowPowerMult);
//    }
//  }
//  motor.stop_all();
//  raiseBasket();
//
//  {
//    unsigned long startTime = millis();
//    while (millis() - startTime < returnTime) {
//      hanMovo.followTape(slowPowerMult);
//    }
//  }
//
//  motor.stop_all();
//  lowerBasket();
//  goto beforeStart;

driveBack:
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);
  
  while (!hanFlyo.cliff()) {
    hanMovo.followTape(slowPowerMult);
    hanMovo.lostAndFindTape();
  }
  delay(200);
  hanMovo.findTapeRight(findTapeWaitTime);

  while (!hanFlyo.cliff()) {
    hanMovo.followTape(slowPowerMult);
  }
  delay(200);
  hanMovo.findTapeLeft(findTapeWaitTime);
  while(!startbutton()){
    hanMovo.followTape(slowPowerMult);
  }
  motor.stop_all();
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
  returnTime = EEPROM[9] * 20;
}

void raiseBasket() {
  motor.speed(scissorLiftMotor, -200);
  while (digitalRead(scissorUpLimitPin) == HIGH);
  motor.speed(scissorLiftMotor, 200);
  delay(10);
  motor.stop(scissorLiftMotor);
}

void lowerBasket() {
  //  LCD.clear();
  //  LCD.print("Lower Basket <Start>");
  //  while (!startbutton());
  //  delay(800);
  //  motor.speed(scissorLiftMotor, 200);
  //  while (!startbutton());
  //  motor.speed(scissorLiftMotor, -255);
  //  motor.stop(scissorLiftMotor);
  //  delay(800);
  motor.speed(scissorLiftMotor, 200);
  while (digitalRead(scissorDownLimitPin) == HIGH);
  motor.speed(scissorLiftMotor, -200);
  delay(10);
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


