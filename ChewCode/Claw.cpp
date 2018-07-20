/*
   Code for the claw functionality of the arduino

   Functions should cover all movements from moving to pick up stuffy,
   to checking if it was picked up, to dropping it off, to returning to
   a rest position, and possibly returning some value to indicate the
   success of the operation
*/

#include "Claw.h"


Claw::Claw(uint16_t objectLim){
  objectLimit = objectLim;
}

boolean Claw::pickUpRight(){
  delay(5000);
}

boolean Claw::pickUpLeft(){
  delay(5000);
}

boolean Claw::pickUpFront(){
  delay(5000);
}

