/*
   Code for the claw functionality of the arduino

   Functions should cover all movements from moving to pick up stuffy,
   to checking if it was picked up, to dropping it off, to returning to
   a rest position, and possibly returning some value to indicate the
   success of the operation
*/
#include <Servo.h>

Servo susan, base, elbow, grip;

#define deLae 500
#define bigDelae 500

#define susanLeft 165
#define baseLeftDown 80
#define elbowLeftDown 180

#define susanFront 20
#define baseFrontDown 70
#define elbowFrontDown 150

#define susanRight 60
#define baseRightDown 100
#define elbowRightDown 180

#define susanBasket 117
#define baseDropoff 0
#define elbowDropoff 0

#define susanTravel 117
#define baseTravel 0
#define elbowTravel 95

#define baseSwivel 0
#define elbowSwivel 0 

#define gripOpen 120
#define gripClose 0

int pos = 0;

void setup() {
  susan.attach(12);
  base.attach(8);
  elbow.attach(9);
  grip.attach(10);

  pickUpLeft();
}

void loop() {

//    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    elbow.write(0);
//    base.write(0);
//    susan.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
//
//  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    elbow.write(0);
//    base.write(0);
//    susan.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
//    delay(500);
  }

// Pick up something from the right
void pickUpRight(){
  moveClaw(susan, susanRight);
  openGrip();
  moveClaw(base, baseRightDown);
  moveClaw(elbow, elbowRightDown);
  closeGrip();
  swivel();
  dropoff();
  return true;
}

// Pickup something from the left
void pickUpLeft(){
  swivel();
  moveClaw(susan, susanLeft);
  openGrip();
  moveClaw(base, baseLeftDown);
  moveClaw(elbow, elbowLeftDown);
  closeGrip();
  swivel();
  dropoff();
  return true;
}

// Pickup something from the front
void pickUpFront(){
  moveClaw(susan, susanFront);
  openGrip();
  moveClaw(base, baseFrontDown);
  moveClaw(elbow, elbowFrontDown);
  closeGrip();
  swivel();
  dropoff();
  return true;
}

// Drops stuff off in the basket
void dropoff(){
  moveClaw(susan, susanBasket);
  moveClaw(base, baseDropoff);
  moveClaw(elbow, elbowDropoff);
  openGrip();
  travel();
  return true;
}

// The position for the claw to be in while we're just running the course
void travel(){
  moveClaw(susan, susanTravel);
  moveClaw(base, baseTravel);
  moveClaw(elbow, elbowTravel);
  closeGrip();
  }

// Opens the grippers
void openGrip(){
  moveClaw(grip, gripOpen);
}

// Closes the grippers
void closeGrip(){
  moveClaw(grip, gripClose);
}

// Position before susan moves so that we don't hit anything
void swivel(){
  moveClaw(base, baseSwivel);
  moveClaw(elbow, elbowSwivel);
  closeGrip();
  }

// Delay to put between servo movements so they're not too jerky
void chill(){
  delay(deLae);
  }

void moveClaw(Servo servo, uint8_t pos){
  servo.write(pos);
  delay(deLae);
}

//boolean Claw::imagineIDidAllThatAndIMissedIt(){
  //return !digitalRead(touchSensorPin);
//}
