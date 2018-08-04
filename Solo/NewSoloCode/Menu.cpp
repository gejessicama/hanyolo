/*
 * knob(7) scrolls through the values to change
    stopbutton() lets you begin and finish editing a single value
    startbutton() exits the menu and begins the robot code


    wish list:
    find tape speed
 */

#include "Menu.h"

static uint8_t menuScreen;
static byte temp;
static const uint8_t menuSize = 10;
static const uint8_t delayTime = 220;

/*
   Displays the EEPROM menu and lets the user edit values.
*/
void Menu::eePromMenu() {
  menuScreen = floor (menuSize * knob(7) / 1024.0);
  switch (menuScreen) {
    case 0 : //142.00
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

    case 1 ://0.76
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

    case 2 :// 50
      displayMenu("PGain", EEPROM[2]);
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

    case 3 ://24
      displayMenu("DGain", EEPROM[3]);
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

    case 4 ://400
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

    case 5 ://620
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

    case 6 ://1250
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

    case 7 ://201
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
    case 8 ://3000
      displayMenu("BackUpRight", EEPROM[8]/100.0);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 100;
          displayMenu("BackUpRi(E)", temp/100.0);
        }
        delay(delayTime);
        EEPROM[8] = temp;
      }
      break;

      case 9 :
      displayMenu("TurningTime", EEPROM[9]*10);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 255;
          displayMenu("TurningT(E)", temp*10);
        }
        delay(delayTime);
        EEPROM[9] = temp;
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
  LCD.println(varName + ": ");
  LCD.print(varValue);
}
