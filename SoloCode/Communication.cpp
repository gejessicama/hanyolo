/*
 * Functions to communicate with the arduino. Must check that there is info to read before calling
 * Uses a digital output pin to send the activation signal to the arduino
 * 
 * THE CLIFF FUNCTIONALITY IS PROBABLY NOT NECESSARY!!!
 */

#include "Communication.h"
#include <phys253.h>

Communication::Communication(char rObject, char lObject, char rCliff, char lCliff){
  rightObject = rObject;
  leftObject = lObject;
  rightCliff = rCliff;
  leftCliff = lCliff;
}

/*
 * Reads in the communication from the Arduino and updates the state of the object
 * requires that it was already checked if there is info
 */
void Communication::readIn(){
  incomingByte = Serial1.read();
  objectToRight = false;
  objectToLeft = false;
  cliffToRight = false;
  cliffToLeft = false;
  if (incomingByte == rightObject){
    objectToRight = true;
  }else if (incomingByte == leftObject){
    objectToLeft = true;
  }else if (incomingByte == rightCliff){
    cliffToRight = true;
  }else if (incomingByte == leftCliff){
    cliffToLeft = true;
  }
}

/*
 * Sends a quick signal to the arduino that it is ready to stop and retrieve a stuffy
 * requires that senderPin is already LOW and that senderPin is an OUTPUT
 */
void Communication::activateClaw(int senderPin){
  digitalWrite(senderPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(senderPin, LOW);
}



