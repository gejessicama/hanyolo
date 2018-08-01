/*
 * knob(7) scrolls through the values to change
    stopbutton() lets you begin and finish editing a single value
    startbutton() exits the menu and begins the robot code
 */

#include "Menu.h"

static uint8_t menuScreen;
static byte temp;
static const uint8_t menuSize = 9;
static const uint8_t delayTime = 220;

/*
   Displays the EEPROM menu and lets the user edit values. Instructions at the top
*/
void Menu::eePromMenu() {
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
      displayMenu("BackupTime", EEPROM[6] * 10);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 255;
          displayMenu("BackupT(E)", temp * 10);
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
    case 8 :
      displayMenu("TimeToIR", EEPROM[8]*20);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 255;
          displayMenu("TimeT(E)", temp*20);
        }
        delay(delayTime);
        EEPROM[8] = temp;
      }
      break;
  }
}

/*
   Displays a given name and value to the LCD
*/
void Menu::displayMenu(String varName, double varValue) {
  delay(1);
  LCD.clear();
  LCD.print(varName + ": ");
  LCD.print(varValue);
}
