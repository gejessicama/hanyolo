/*
   Code for the claw functionality of the arduino

   Functions should cover all movements from moving to pick up stuffy,
   to checking if it was picked up, to dropping it off, to returning to
   a rest position, and possibly returning some value to indicate the
   success of the operation
*/

#include "Claw.h"
#include "Servo.h"

#define susanFront 0 
#define susanLeft 45
#define susanBasket 90
#define susanRight 135

#define baseLeftDown 80
#define baseRightDown 80
#define baseFrontDown 80
#define baseSwivel 0
#define baseDropoff 20

#define elbowLeftDown 150
#define elbowRightDown 150
#define elbowFrontDown 150
#define elbowSwivel 0 
#define elbowDropoff 110

#define gripOpen 120
#define gripClose 0

Claw::Claw(){
  
}

boolean Claw::pickUpRight(){
  
}

boolean Claw::pickUpLeft(){
  
}

boolean Claw::pickUpFront(){
  
}

boolean Claw::dropoff(){

}

void Claw::open(){

}

void Claw::close(){

}