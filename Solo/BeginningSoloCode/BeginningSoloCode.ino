
/*
   Code for the TINAH (Solo) that will only include up to the third stuffy, but can be added to as we gain more functionality

   Certian Values are stored in EEPROM, and there is a menu system for reading these in:
    knob(7) scrolls through the values to change
    stopbutton() lets you begin and finish editing a single value
    startbutton() exits the menu and begins the robot code
*/

#include <phys253.h>
#include <EEPROM.h>
#include "Motion.h"
#include "Crossing.h"
#include "Constants.h"

// VARIABLES FOR EEPROM MENU
uint8_t menuScreen;
byte temp;
const uint8_t menuSize = 8;
const uint8_t delayTime = 220;

// OTHER VARIABLES
volatile uint8_t state = 0;
volatile uint8_t pstate = 0;
volatile uint8_t rememberState;
volatile uint16_t rightWheelDist, leftWheelDist;

byte baseSpeed, proportionalGain, derivativeGain;
double powerMult;
int onTape, overCliff, backupTime;

Motion hanMovo(rightMotor, leftMotor);
Crossing hanFlyo(rightMotor, leftMotor, rightMostQRD, leftMostQRD,irSignalPin);

//  HELPER FUNCTIONS
void updateState();
void raiseBasket();
void lowerBasket();

//  INTERRUPT FUNCTIONS
void changeState();

void setup() {
  LCD.begin();
  LCD.print("Setup");
  pinMode(fromChewPin, INPUT);
  pinMode(toChewPin, OUTPUT);
}
long startTime = millis();


void loop() {
  LCD.print(state);
  switch (state) {

    case 0 : // START BUTTON NOT YET PRESSED
      while (!startbutton()) {
        eePromMenu();
      }
      delay(200);
      hanMovo.setConstants();
      hanFlyo.setConstants();
      saveMenuValues();
      attachInterrupt(fromChewPin, changeState, CHANGE);//change does not work 
      state++;
      
      break;

    case 1 : // STARTING STATE UNTIL FIRST GAP
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
      LCD.print("Move");
      //hanMovo.driveMotors();
      LCD.clear();
      //hanMovo.followRightEdge(rightOutQRD,rightInQRD,pGainConst, dGainConst);

      
      if (hanFlyo.cliff()) { // detect cliff then reverse for bt time
        hanFlyo.dropBridge(1000,110);
        state = 3;
      }


      break;
    case 2 :
      motor.speed(rightMotor, -240);
      motor.speed(leftMotor, 255);
      delay(300);
      motor.stop(rightMotor);
      motor.stop(leftMotor);
      motor.stop_all();
      LCD.clear();
      LCD.print("Pick Up Stuffy");
      break;


    case 3 :
      long st = millis();
      long et = st;
      while (et - st < 2000.0) {
        hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
      }
      motor.speed(rightMotor,-255);
      motor.speed(leftMotor, 255);
      motor.stop_all();
      while (!hanFlyo.detect10KIR()) {
        LCD.print("1k");
      }
      LCD.print("10k");
      break;

  }
}

void updateState() {
  // Changed to Low then HIGH
  digitalWrite(toChewPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(toChewPin, LOW);
  if (state == 0) {
    state = 1;
  } else {
    state = 0;
  }
}

// INTERRUPT FUNCTIONS
void changeState() {
  if (state == 2) {
    state = 1;
    //    state = rememberState;
  } else {
    //    rememberState = state;
    state = 2;
  }
}

///*
// * Modifies EEPROM values and saves them to a more descriptive variable name
// */
void saveMenuValues(){
  baseSpeed = EEPROM[0];
  powerMult = EEPROM[1]/100.0;
  proportionalGain = EEPROM[2];
  derivativeGain = EEPROM[3];
  onTape = EEPROM[4]*10;
  overCliff = EEPROM[5]*10;
  backupTime = EEPROM[6]*3;
}

/*
 * Displays the EEPROM menu and lets the user edit values. Instructions at the top
 */
void eePromMenu() {
  menuScreen = floor (menuSize * knob(7) / 1024.0);
  switch (menuScreen) {
    case 0 :
      displayMenu("BaseSpeed", EEPROM[0]);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 255;
          displayMenu("BaseSp(E)", temp);
        }
        delay(delayTime);
        EEPROM[0] = temp;
      }
      break;

    case 1 :
      displayMenu("PowerMult", EEPROM[1] / 100.0);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 100;
          displayMenu("PowerM(E)", temp / 100.0);
        }
        delay(delayTime);
        EEPROM[1] = temp;
      }
      break;

    case 2 :
      displayMenu("ProportionalGain", EEPROM[2]);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 255;
          displayMenu("ProportionalG(E)", temp);
        }
        delay(delayTime);
        EEPROM[2] = temp;
      }
      break;

    case 3 :
      displayMenu("DerivativeGain", EEPROM[3]);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 255;
          displayMenu("DerivativeG(E)", temp);
        }
        delay(delayTime);
        EEPROM[3] = temp;
      }
      break;

    case 4 :
      displayMenu("OnTape", EEPROM[4] * 10);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 150;
          displayMenu("OnT(E)", temp * 10);
        }
        delay(delayTime);
        EEPROM[4] = temp;
      }
      break;

    case 5 :
      displayMenu("OverCliff", EEPROM[5] * 10);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 150;
          displayMenu("OverCl(E)", temp * 10);
        }
        delay(delayTime);
        EEPROM[5] = temp;
      }
      break;

    case 6 :
      displayMenu("BackupTime", EEPROM[6] * 3);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 255;
          displayMenu("BackupT(E)", temp * 3);
        }
        delay(delayTime);
        EEPROM[6] = temp;
      }
      break;

          case 7 :
      displayMenu("BackupSpeed", EEPROM[7]);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 255;
          displayMenu("BackupSp(E)", temp);
        }
        delay(delayTime);
        EEPROM[7] = temp;
      }
      break;
  }
}

/*
 * Displays a given name and value to the LCD
 */
void displayMenu(String varName, double varValue) {
  delay(1);
  LCD.clear();
  LCD.print(varName + ": ");
  LCD.print(varValue);
}

