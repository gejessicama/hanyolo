/*
   Main code for the TINAH microprocessor (aka Solo)
   Contains a sequence of actions to be performed by the robot, and a couple of
   helper functions that didn't fit with any of our helper classes
*/

#include <phys253.h>
#include <EEPROM.h>
#include "Motion.h"
#include "Crossing.h"
#include "Constants.h"
#include "Menu.h"

uint8_t stuffyDelay;
uint16_t firstBackupTime, findTapeTime, afterCliffTime;
double regularPowerMult, slowPowerMult, backupPowerMult, rightWheelPercent, rampPowerMult;

byte baseSpeed;
double powerMult;

/*
   Sets up our Liquid Crystal Display, declares our inputs and our outputs
*/
void setup() {
  LCD.begin();
  LCD.clear();

  pinMode(fromChewPin, INPUT);
  pinMode(irSignalPin, INPUT);
  pinMode(toChewPinLeft, OUTPUT);
  pinMode(toChewPinRight, OUTPUT);
}

/*
   Our main function:
   Though the function automatically loops back on itself,
   we opted to use our own loops for better control over the robot's actions
*/
void loop() {

  /* Before we start, we want a chance to set things up, and edit values stored in memory */
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


  /* Actions that take us from the start to the first gap */
firstEwok:
  // tells the Arduino (Chewy) to only look on the right for Ewoks
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

  // If we see the first Ewok, then we will drive more slowly as we approach the cliff
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


  /* Steps for dropping and crossing our bridge */
firstBridge:
  // tells the Arduino (Chewy) not to look for Ewoks
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);

  hanMovo.driveMotors(-backupPowerMult * rightWheelPercent, -backupPowerMult);
  delay(firstBackupTime);
  motor.stop_all();

  hanFlyo.dropBridge();
  hanMovo.driveMotors(-slowPowerMult, -slowPowerMult);
  delay(250);
  motor.stop_all();
  delay(20);

  hanMovo.driveMotors(slowPowerMult, slowPowerMult);
  while (!hanFlyo.cliff());
  delay(200);

  // Our bridge has two "cliffs" on it to mark the beginning and end of the bridge
  // This is more helpful for crossing it on our way back
  while (!hanFlyo.cliff());
  delay(afterCliffTime);


  /* Actions for picking up the second stuffy */
secondStuffy:
  // tells Chewy to only look on the right for Ewoks
  digitalWrite(toChewPinRight, HIGH);
  digitalWrite(toChewPinLeft, LOW);
  hanMovo.findTapeRight(findTapeWaitTime);

  while (digitalRead(fromChewPin) == LOW) {
    hanMovo.followTape(slowPowerMult);
  }
  hanMovo.driveMotors(regularPowerMult, -regularPowerMult);
  delay(stuffyDelay);
  hanMovo.stopMotors();

  while (digitalRead(fromChewPin) == HIGH);


  /* Deals with detecting a change in frequency with the IR signal */
toTheIR:
  // tells Chewy not to look for stuffies
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);

  if (hanFlyo.detect10KIR()) {
    LCD.clear();
    LCD.print("IR 10k p1");
    while (hanFlyo.detect10KIR());
  }
  while (!hanFlyo.detect10KIR());


stormtrooperRoom:
  // additional command for ease of jumping to this label when we were testing our code
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);

  // Originally had additional commands for better aligning ourselves to cross under the archway
  //hanMovo.driveMotors(slowPowerMult, 0);
  //delay(100);
  hanMovo.findTapeRight(findTapeTime);

  // We have to wait to pass by the stormtroopers so we don't mistake them for Ewoks
  {
    unsigned long startTime = millis();
    while (millis() - startTime < 3000) { //should be enough time to put us on the platform
      hanMovo.followTape(slowPowerMult);
    }
  }


/* Actions related to picking up the third Ewok and not falling off a cliff */
secondCliff:
  // Tells Chewy to look for Ewoks on the Left only
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, HIGH);

  while (!hanFlyo.cliff()) {
    // Going slowly helps us not accidentally rotate a spinning platform
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


/* How we back up, and turn around to find the tape again */
turnAround:
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);
  hanMovo.driveMotors(-backupPowerMult, -backupPowerMult);
  delay(firstBackupTime);

  hanMovo.driveMotors(slowPowerMult, -slowPowerMult);
  delay(500);
  hanMovo.findTapeLeft(5000);


/* Commands for driving all the way back to the start */
driveBack:
  digitalWrite(toChewPinRight, LOW);
  digitalWrite(toChewPinLeft, LOW);

  while (!hanFlyo.cliff()) {
    hanMovo.followTape(slowPowerMult);
    // In case we rotated the platform, we need to be able to get back on tape
    hanMovo.lostAndFindTape();
  }
  delay(200);
  hanMovo.findTapeRight(findTapeTime);

  // Our bridge has two "cliffs" to signal its begininng and end
  // We follow tape to stay in the middle of the bridge
  while (!hanFlyo.cliff()) {
    hanMovo.followTape(slowPowerMult);
  }
  delay(500);
  hanMovo.findTapeLeft(findTapeTime);

  // We stop after getting back into the start area (which ends in the edge of the table)
  while (!hanFlyo.cliff()) {
    hanMovo.followTape(slowPowerMult);
  }
  motor.stop_all();
  goto beforeStart;

}

/*
 * Stores all the values we need from the TINAH memory
 */
void saveMenuValues() {
  regularPowerMult = EEPROM[2] / 100.0;
  slowPowerMult = EEPROM[3] / 100.0;
  backupPowerMult = EEPROM[4] / 100.0;
  rightWheelPercent = EEPROM[5] / 100.0;
  firstBackupTime = EEPROM[6] * 10;
  stuffyDelay = EEPROM[7];
  rampPowerMult = EEPROM[8] / 100.0;
  findTapeTime = EEPROM[9] * 20;
  afterCliffTime = EEPROM[10] * 10;
}

/*
 * Displays all our QRD values so we can easily see how ambient light will affect our performance
 */
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


