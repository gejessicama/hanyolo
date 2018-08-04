#include<Arduino.h>
#include<phys253.h>
#include <EEPROM.h>
#ifndef MENU_H
#define MENU_H

class Menu {
  private:
    static void displayMenu(String, double);

  public:
    static void eePromMenu();

};

#endif
