/*
 * The idea is that this would have the communication functions
 * Perhaps one function is called to read in and store whatever info was just passed
 * through serial
 * and then subsequent functions can acess the stored data about what info was sent and what we may
 * be sensing
 */

#include "Communication.h"
#include <phys253.h>

Communication::Communication(char rObject, char lObject){
  objectToRight = false;
  objectToLeft = false;
  rightObject = rObject;
  leftObject = lObject;
}

/*
 * Reads in the communication from the Arduino and updates the state of the object
 * requires that it was already checked if there is info
 */
void Communication::readIn(){
  incomingByte = Serial1.read();
  if (incomingByte == rightObject){
    objectToRight = true;
  }else if (incomingByte == leftObject){
    objectToLeft = true;
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



