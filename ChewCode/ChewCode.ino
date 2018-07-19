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

#include "Claw.h"

#define triggerPinRight 8
#define echoPinRight 9
#define triggerPinLeft 10
#define echoPinLeft  11
#define triggerPinFront 12
#define echoPinFront 13
#define interruptPin 0
#define objectLimit 400

volatile uint8_t state = 0;

boolean readInSonar(uint8_t, uint8_t, uint16_t);
void updateState();

void setup() {
  pinMode(triggerPinRight, OUTPUT);
  pinMode(triggerPinLeft, OUTPUT);
  pinMode(triggerPinFront, OUTPUT);
  attachInterrupt(interruptPin, updateState, RISING);
}

void loop() {

  switch (state) {
    case 0 : // START BUTTON NOT YET PRESSED
      break;
      
    case 1 : // STARTING STATE UNTIL FIRST GAP
      if (readInSonar(triggerPinRight, echoPinRight, objectLimit)) {
        //tell solo to wait
        //activate right claw pick up
        //tell solo to go
      }
      break;
      
    case 2 : // CROSSING THE FIRST GAP
      if (readInSonar(triggerPinRight, echoPinRight, objectLimit)) {
        //tell solo to wait
        //activate right claw pick up
        //tell solo to go
      }
      break;
      
    case 3 : // IR BEACON JUST CHANGED FREQUENCY
      //ignore for a certain amount of hard coded time
      // (we need to pass the arch and the stormtroopers)
      if (readInSonar(triggerPinLeft, echoPinLeft, objectLimit)) {
        //tell solo to wait
        //activate left claw pick up
        //tell solo to go
      }
      break;

    // COULD ADD ANOTHER CASE HERE FOR WHEN WE ARE PAST THE STORMTROOPERS
      
    case 4 : // WE JUST LEFT THE TAPE AND TURNED TOWARDS THE SECOND GAP
      break;
      
    case 5 : // WE JUST DROPPED THE SECOND BRIDGE
      break;
      
    case 6 : // WE JUST CROSSED OUR SECOND BRIDGE
      if (readInSonar(triggerPinFront, echoPinFront, objectLimit)) {
        //tell solo to wait
        //activate front claw pick up
        //tell solo to go
      }
      break;
      
    case 7 : // WE JUST TURNED TOWARDS THE SECOND TOWER
      if (readInSonar(triggerPinLeft, echoPinLeft, objectLimit)) {
        //tell solo to wait
        //activate left claw pick up
        //tell solo to go
      }
      break;
    case 8 : // COMPLETED THE COURSE MAYBE?
      break;
      
  }
}

boolean readInSonar(uint8_t trig, uint8_t echo, uint16_t objLim){
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10);  
  digitalWrite(trig, LOW);
  return (pulseIn(echo, HIGH) < objLim);
}

//  INTERRUPT FUNCTION
void updateState() {
  state++;
}


