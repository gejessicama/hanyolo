/*
   Code for the Arduino
   The Arduino (Chew) controls our Sonar sensing and sends this information to the TINAH
   It also listens for the TINAH to tell it to activate the claw
   The arduino then activates the claw before telling the TINAH it is done and then continues

   For now, Chew will constantly be reading in the sonars and sending that information to Solo
   Late may want to change how it sends info

   Need more communication from TINAH to tell us when to ignore an object

   or: read in an object and check for a claw signal for a certain amount of time. If no signal is
   recieved in that time, then give up and resume as normal
*/

#include "Sonar.h"

#define triggerPinRight 8
#define echoPinRight 9
#define triggerPinLeft 10
#define echoPinLeft  11
#define listenPin 2
#define objectLim 400

//make sure these are the same as defined for Solo
#define rightObjectChar 'r'
#define leftObjectChar 'l'
#define frontObjectChar 'f'

volatile uint8_t state = 0;

Sonar chewy(objectLim);

void updateState();

void setup() {
  Serial.begin(9600);
  pinMode(triggerPinRight, OUTPUT);
  pinMode(triggerPinLeft, OUTPUT);
  attachInterrupt(listenPin, updateState, RISING);
}

void loop() {

  //ignores state = 0, 3, 5, 8

  switch (state) {
    case 1 : // STARTING STATE UNTIL FIRST GAP
      if (chewy.readIn(triggerPinRight, echoPinRight)) {
        Serial.print(rightObjectChar);
        //activate claw sequence
      }
      break;
    case 2 : //we need to cross the gap
      if (chewy.readIn(triggerPinRight, echoPinRight)) {
        Serial.print(rightObjectChar);
        //activate claw sequence
      }
      break;
    case 4 : // we are now in the stormtrooper room
      //ignore for a certain amount of hard coded time
      // (we need to pass the arch and the stormtroopers)
      if (chewy.readIn(triggerPinLeft, echoPinLeft)) {
        Serial.print(leftObjectChar);
        //activate claw sequence
      }
      break;
    case 6 : // we just reached first tower
      //read in sonar in front
      break;
    case 7 : //we are on the second tower
      if (chewy.readIn(triggerPinRight, echoPinRight)) {
        Serial.print(rightObjectChar);
        //activate claw sequence
      }
      break;
  }
}

void updateState() {
  state++;
}
