
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
#include "Menu.h"


// OTHER VARIABLES
volatile uint8_t state = 0;
volatile uint8_t rememberState;

byte baseSpeed, proportionalGain, derivativeGain;
double powerMult;
int onTape, overCliff, backupTime;

Motion hanMovo(rightMotor, leftMotor);
Crossing hanFlyo(rightMotor, leftMotor, rightMostQRD, leftMostQRD, irSignalPin);

//  HELPER FUNCTIONS
void updateState();
void raiseBasket();
void lowerBasket();

//
//// INTERRUPT FUNCTION
//void changeState() {
//  if (state == 2) {
//    state = 1;
//    //    state = rememberState;
//  } else {
////    motor.speed(rightMotor, -240);
////    motor.speed(leftMotor, 255);
////    delay(300);
//    motor.speed(rightMotor,0);
//    motor.speed(leftMotor,0);
//    motor.stop_all();
//    //    rememberState = state;
//    state = 2;
//  }
//}


void setup() {
  LCD.begin();
  LCD.print("Setup");
  Serial.begin(9600);
  pinMode(fromChewPin, INPUT);
  pinMode(toChewPin, OUTPUT);
  pinMode(irSignalPin, INPUT);
}


void loop() {

  switch (state) {

    case 0 : // START BUTTON NOT YET PRESSED
      while (!startbutton()) {
        Menu::eePromMenu();
      }
      delay(1000);

      hanMovo.setConstants();
      hanFlyo.setConstants();

      //attachInterrupt(fromChewPin, changeState, CHANGE);//change does not work
      state = 1;
      break;

    case 1 : // STARTING STATE UNTIL FIRST GAP
 //       Serial.println(digitalRead(fromChewPin));
      LCD.clear();
      LCD.print("Move");
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
//

      while (digitalRead(fromChewPin) == HIGH) {
        motor.stop_all();
        LCD.clear();
        LCD.print("Pick up Stuffy");
      }


//      if (hanFlyo.cliff()) { // detect cliff then reverse for bt time
//        hanFlyo.dropBridge(1000, 110);
//        state = 3;
//      }
//      break;

//    case 2 :
//      LCD.clear();
//      LCD.print("Pick Up Stuffy");
//      break;

    case 4 :
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
      if (hanFlyo.cliff());
      break;

    case 3 :
      long st = millis();
      long et = st;
      while (et - st < 1250.0) {
        hanMovo.followTape(rightMiddleQRD, leftMiddleQRD);
        et = millis();
      }
      //      motor.speed(rightMotor,-255);
      //      motor.speed(leftMotor, 255);
      motor.speed(rightMotor, -255);
      motor.speed(leftMotor, 255);
      motor.stop_all();
      while (!hanFlyo.detect10KIR()) {
        LCD.print("not 10k");
        LCD.clear();
      }
      LCD.clear();
      LCD.print("10k");
      delay(1000);
      state = 4;
      break;

  }
}



