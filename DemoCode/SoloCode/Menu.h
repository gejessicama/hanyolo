/*
   Menu is a class with no objects whose only function is to allow us to edit the values
   stored in the TINAH's memory with calls to EEPROM. Doing this allows us to change
   variables without uploading new code.
*/


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
