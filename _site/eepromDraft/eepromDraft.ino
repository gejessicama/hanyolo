/*
 * EEPROM code for the TINAH
 * 
 * BaseSpeed
 * PowerMultiplier
 * ProportionalGain
 * DerivativeGain
 * OnTape
 * OverCLiff
 * BackupTime
 * 
 * Knob 6 scrolls through the menu
 * Knob 7 adjusts the value
 */
#include <phys253.h>

uint8_t menuSize;
uint8_t menuScreen;

byte baseSpeed = EEPROM[0]; //will never be more that 255
double powerMultiplier = EEPROM[1] / 100.0;
byte proportionalGain = EEPROM[2];
byte derivativeGain = EEPROM[3];
int onTape = EEPROM[4]  * 100;
int overCliff = EEPROM[5] * 100;
int backUpTime = EEPROM[6] * 100;



void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

void eePromMenu(){
  LCD.clear();
  menuScreen = floor (menuSize * knob(6) / 1024.0);
  if(menuScreen == 0){
    LCD.print("BaseSpeed: ");
    LCD.print(EEPROM[0]);
  }
  else if (menuScreen == 1)j
  
}

