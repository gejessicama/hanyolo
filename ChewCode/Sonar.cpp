/*
 * Code for the Arduino to read in and process the Sonar values
 */

#include "Sonar.h"

Sonar::Sonar(uint16_t objLim){
  objectLimit = objLim;
}

boolean Sonar::readIn(uint8_t triggerPin, uint8_t echoPin){
  return (getDistance(triggerPin, echoPin) < objectLimit);
}

uint16_t Sonar::getDistance(uint8_t trig, uint8_t echo){
    digitalWrite(trig, HIGH); 
    delayMicroseconds(10);  
    digitalWrite(trig, LOW);
    return pulseIn(echo, HIGH);
}
