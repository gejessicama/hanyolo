/*
   INSTRUCTIONS
    knob(7) scrolls through the values to change
    stopbutton() lets you begin and finish editing a single value
    startbutton() exits the menu and begins the robot code

   CURRENT VALUES
    ONTAPE: 400
    OVERCLIFF: 470
    NORMAL POWER: 0.79
    SLOW POWER: 0.51
    BACKUP POWER: 1.07
    RIGHT PERCENTAGE: 0.74
    FIRST BACKUP TIME: 450
    STUFFY DELAY: 21
    RAMP POWER: 0.99
    RETURN TIME: 3400
*/

#include "Menu.h"

static uint8_t menuScreen;
static byte temp;
static const uint8_t menuSize = 11;
static const uint8_t delayTime = 250;

/*
   Displays the EEPROM menu and lets the user edit values.
*/
void Menu::eePromMenu() {
  menuScreen = floor (menuSize * knob(7) / 1024.0);
  switch (menuScreen) {

    case 0 ://400
      displayMenu("OnTape", EEPROM[0] * 10);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 150;
          displayMenu("OnT(E)", temp * 10);
        }
        delay(delayTime);
        EEPROM[0] = temp;
      }
      break;

    case 1 ://620
      displayMenu("OverCliff", EEPROM[1] * 10);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 150;
          displayMenu("OverCl(E)", temp * 10);
        }
        delay(delayTime);
        EEPROM[1] = temp;
      }
      break;

    case 2 :
      displayMenu("Normal Power", EEPROM[2] / 100.0);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 200;
          displayMenu("Normal Po(E)", temp / 100.0);
        }
        delay(delayTime);
        EEPROM[2] = temp;
      }
      break;

    case 3 :
      displayMenu("Slow Power", EEPROM[3] / 100.0);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 200;
          displayMenu("Slow Po(E)", temp / 100.0);
        }
        delay(delayTime);
        EEPROM[3] = temp;
      }
      break;

    case 4 :// 50
      displayMenu("Backup Power", EEPROM[4] / 100.0);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 200;
          displayMenu("Backup Po(E)", temp / 100.0);
        }
        delay(delayTime);
        EEPROM[4] = temp;
      }
      break;

    case 5 :
      displayMenu("Right Mult", EEPROM[5] / 100.0);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 200;
          displayMenu("Right M(E)", temp / 100.0);
        }
        delay(delayTime);
        EEPROM[5] = temp;
      }
      break;

    case 6 :
      displayMenu("1st Back Time", EEPROM[6] * 10);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 255;
          displayMenu("1st Back T(E)", temp * 10);
        }
        delay(delayTime);
        EEPROM[6] = temp;
      }
      break;

    case 7 :
      displayMenu("Stuffy Delay", EEPROM[7]);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 255;
          displayMenu("Stuffy De(E)", temp);
        }
        delay(delayTime);
        EEPROM[7] = temp;
      }
      break;

    case 8 :
      displayMenu("Ramp Power", EEPROM[8] / 100.0);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 255;
          displayMenu("Ramp Po(E)", temp / 100.0);
        }
        delay(delayTime);
        EEPROM[8] = temp;
      }
      break;

    case 9 :
      displayMenu("Find Tape Time", EEPROM[9] * 20);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 255;
          displayMenu("Find Tape T(E)", temp * 20);
        }
        delay(delayTime);
        EEPROM[9] = temp;
      }
      break;

    case 10 :
      displayMenu("After Cliff", EEPROM[10] * 10);
      if (stopbutton()) {
        delay(delayTime);
        while (!stopbutton()) {
          temp = knob(6) / 1024.0 * 255;
          displayMenu("After Cl(E)", temp * 10);
        }
        delay(delayTime);
        EEPROM[10] = temp;
      }
      break;
  }
}

/*
   Displays a given name and value to the LCD
*/
void Menu::displayMenu(String varName, double varValue) {
  delay(2);
  LCD.clear();
  LCD.print(varName + ": ");
  LCD.setCursor(0, 1);
  LCD.print(varValue);
}
