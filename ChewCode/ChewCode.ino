/*
 * Code for the Arduino
 * The Arduino (Chew) controls our Sonar sensing and sends this information to the TINAH
 * It also listens for the TINAH to tell it to activate the claw
 * The arduino then activates the claw before telling the TINAH it is done and then continues
 * 
 * For now, Chew will constantly be reading in the sonars and sending that information to Solo
 * Late may want to change how it sends info
 * 
 * Need more communication from TINAH to tell us when to ignore an object
 * 
 * or: read in an object and check for a claw signal for a certain amount of time. If no signal is
 * recieved in that time, then give up and resume as normal
 */
 
#include "Sonar.h"

#define triggerPinRight 8
#define echoPinRight 9
#define triggerPinLeft 10
#define echoPinLeft  11
#define listenPin 2
#define objectLim 400

bool objectOnRight = false, objectOnLeft = false;

Sonar chewy(objectLim);

 
void setup(){
    Serial.begin(9600); 
    pinMode(triggerPinRight, OUTPUT); 
    pinMode(triggerPinLeft, OUTPUT);  
}

void loop(){ 
  if(!objectOnRight && chewy.readIn(triggerPinRight, echoPinRight)){ //current state is false, but new state is true
    objectOnRight = true;
  }
  else if (!objectOnLeft && chewy.readIn(triggerPinLeft, echoPinLeft)){
    objectOnLeft = true;
  }
  
  //I want to read in the sonar and check if it is different than my current left and right state
  // if the state goes from false to true, I want to send this info to Solo
  // I want to store any state changes as my current state
  // after I sent info to Solo, the only thing I want to do is listen for the signal to activate my claw
  //after I carry out my claw function I go back to normal
}


