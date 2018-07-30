/*
   EEPROM code for the TINAH

   BaseSpeed
   PowerMultiplier
   ProportionalGain
   DerivativeGain
   OnTape
   OverCLiff
   BackupTime

   Knob 7 scrolls through the menu
   stop button lets you change and then saves the value of a variable
   Knob 7 adjusts the value

   go to menu first, when done with menu press start to exit. Start will update the state and store all the values
   that are in the EEPROM to their respective variable in our code. or just access eeprom directly
   look at efficiencies
*/
#include <phys253.h>
#include <EEPROM.h>


uint8_t menuScreen;
uint8_t state = 0;
byte temp;
const uint8_t menuSize = 7;
const uint8_t delayTime = 200;

//byte baseSpeed = EEPROM[0]; //will never be more that 255
//double powerMultiplier = EEPROM[1] / 100.0;
//byte proportionalGain = EEPROM[2];
//byte derivativeGain = EEPROM[3];
//int onTape = EEPROM[4]  * 100;
//int overCliff = EEPROM[5] * 100;
//int backUpTime = EEPROM[6] * 100;



void setup() {
  // put your setup code here, to run once:
  LCD.begin();

}

void loop() {
  switch (state){
    case 0 : 
      while(!startbutton()){
        eePromMenu();
      }
      state ++;
      break;

      case 1 :
      LCD.clear();
      LCD.print("finished setup");
  }

}

// use knob(7) to scroll through all the variables, press stop to edit, press start to finish editing and save

void eePromMenu() {
 menuScreen = floor (menuSize * knob(7) / 1024.0);
  switch (menuScreen) {
    case 0 :
      displayMenu("BaseSpeed", EEPROM[0]);
      if(stopbutton()){
        delay(delayTime);
        while(!stopbutton()){
          temp = knob(6) / 1024.0 * 255;
          displayMenu("BaseSp(E)", temp);
        }
        delay(delayTime);
        EEPROM[0] = temp;
      }
      break;
      
    case 1 :
      displayMenu("PowerMult", EEPROM[1]/100.0);
      if(stopbutton()){
        delay(delayTime);
        while(!stopbutton()){
          temp = knob(6) / 1024.0 * 100;
          displayMenu("PowerM(E)", temp/100.0);
        }
        delay(delayTime);
        EEPROM[1] = temp;
      }
      break;
      
      case 2 :
      displayMenu("ProportionalGain", EEPROM[2]);
      if(stopbutton()){
        delay(delayTime);
        while(!stopbutton()){
          temp = knob(6) / 1024.0 * 255;
          displayMenu("ProportionalG(E)", temp);
        }
        delay(delayTime);
        EEPROM[2] = temp;
      }
      break;
      
      case 3 :
      displayMenu("DerivativeGain", EEPROM[3]);
      if(stopbutton()){
        delay(delayTime);
        while(!stopbutton()){
          temp = knob(6) / 1024.0 * 255;
          displayMenu("DerivativeG(E)", temp);
        }
        delay(delayTime);
        EEPROM[3] = temp;
      }
      break;
      
      case 4 :
      displayMenu("OnTape", EEPROM[4] * 10);
      if(stopbutton()){
        delay(delayTime);
        while(!stopbutton()){
          temp = knob(6) / 1024.0 * 150;
          displayMenu("OnT(E)", temp*10);
        }
        delay(delayTime);
        EEPROM[4] = temp;
      }
      break;
      
      case 5 :
      displayMenu("OverCliff", EEPROM[5]*10);
      if(stopbutton()){
        delay(delayTime);
        while(!stopbutton()){
          temp = knob(6) / 1024.0 * 150;
          displayMenu("OverCl(E)", temp*10);
        }
        delay(delayTime);
        EEPROM[5] = temp;
      }
      break;
      
      case 6 :
      displayMenu("BackupTime", EEPROM[6]*3);
      if(stopbutton()){
        delay(delayTime);
        while(!stopbutton()){
          temp = knob(6) / 1024.0 * 255;
          displayMenu("BackupT(E)", temp*3);
        }
        delay(delayTime);
        EEPROM[6] = temp;
      }
      break;
  }
}

void displayMenu(String varName, double varValue){
  delay(1);
  LCD.clear();
  LCD.print(varName + ": ");
  LCD.print(varValue);
}

