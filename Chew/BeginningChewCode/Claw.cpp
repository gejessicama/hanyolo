/*
   Code for the claw functionality of the arduino

   Functions should cover all movements from moving to pick up stuffy,
   to checking if it was picked up, to dropping it off, to returning to
   a rest position, and possibly returning some value to indicate the
   success of the operation
*/

#include "Claw.h"

#define deLae 500
#define bigDelae 500

#define susanLeft 165
#define susanFront 20
#define susanRight 60
#define susanBasket 117
#define susanTravel 117

#define baseLeftDown 80
#define baseFrontDown 70
#define baseRightDown 100
#define baseDropoff 0
#define baseTravel 0
#define baseSwivel 0

#define elbowLeftDown 180
#define elbowFrontDown 150
#define elbowRightDown 180
#define elbowDropoff 0
#define elbowTravel 95
#define elbowSwivel 0

#define gripOpen 120
#define gripClose 0


Claw::Claw(Servo susanPin, Servo basePin, Servo elbowPin, Servo gripPin){
  susan = susanPin;
  base = basePin;
  elbow = elbowPin;
  grip = gripPin;
}

// Pick up something from the right
void Claw::pickUpRight(){
  moveClaw(susan, susanRight);
  openGrip();
  moveClaw(base, baseRightDown);
  moveClaw(elbow, elbowRightDown);
  closeGrip();
  swivel();
  dropoff();
   
}

// Pickup something from the left
void Claw::pickUpLeft(){
  swivel();
  moveClaw(susan, susanLeft);
  openGrip();
  moveClaw(base, baseLeftDown);
  moveClaw(elbow, elbowLeftDown);
  closeGrip();
  swivel();
  dropoff();
   
}

// Pickup something from the front
void Claw::pickUpFront(){
  moveClaw(susan, susanFront);
  openGrip();
  moveClaw(base, baseFrontDown);
  moveClaw(elbow, elbowFrontDown);
  closeGrip();
  swivel();
  dropoff();
   
}

// Drops stuff off in the basket
void Claw::dropoff(){
  moveClaw(susan, susanBasket);
  moveClaw(base, baseDropoff);
  moveClaw(elbow, elbowDropoff);
  openGrip();
  travel();
   
}

// The position for the claw to be in while we're just running the course
void Claw::travel(){
  moveClaw(susan, susanTravel);
  moveClaw(base, baseTravel);
  moveClaw(elbow, elbowTravel);
  closeGrip();
  }

// Opens the grippers
void Claw::openGrip(){
  moveClaw(grip, gripOpen);
}

// Closes the grippers
void Claw::closeGrip(){
  moveClaw(grip, gripClose);
}

// Position before susan moves so that we don't hit anything
void Claw::swivel(){
  moveClaw(base, baseSwivel);
  moveClaw(elbow, elbowSwivel);
  closeGrip();
  }

void Claw::moveClaw(Servo servo, uint8_t pos){
  servo.write(pos);
  delay(deLae);
}

