/*
   Code for the claw functionality of the arduino

   Functions should cover all movements from moving to pick up stuffy,
   to checking if it was picked up, to dropping it off, to returning to
   a rest position, and possibly returning some value to indicate the
   success of the operation
*/
#include <Servo.h>

Servo susan, base, elbow, grip;

#define rightTriggerPin 4
#define rightEchoPin 5
#define leftTriggerPin 6
#define leftEchoPin  7

#define deLae 700
#define bigDelae 500

#define susanRight 160
#define baseRightDown 100
#define elbowRightDown 180

#define susanFront 108
#define baseFrontDown 70
#define elbowFrontDown 150

#define susanLeft 60
#define baseLeftDown 90
#define elbowLeftDown 180

#define susanBasket 20
#define baseDropoff 0
#define elbowDropoff 0

#define susanTravel 20
#define baseTravel 0
#define elbowTravel 0

#define baseSwivel 0
#define elbowSwivel 0 

#define gripOpen 120
#define gripClose 0

const uint16_t objectLimit = 400;
const uint16_t stuffyLimit = 60000;

void setup() {
  pinMode(rightTriggerPin, OUTPUT);
  pinMode(leftTriggerPin, OUTPUT);
  
  susan.attach(12);
  base.attach(8);
  elbow.attach(9);
  grip.attach(10);

  //Serial.begin(9600);

  pickUpLeft();
  pickUpRight();
}
void loop() {

  //Serial.println(readSonar(rightTriggerPin, rightEchoPin));
  //Serial.println(readSonar(leftTriggerPin, leftEchoPin));

  if (readInSonar(rightTriggerPin, rightEchoPin)){
    pickUpLeft();
    }
  if (readInSonar(leftTriggerPin, leftEchoPin)){
    pickUpRight();
  }
  else {
    travel();
  }
}

boolean readInSonar(uint8_t trig, uint8_t echo){
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10);  
  digitalWrite(trig, LOW);
  return (pulseIn(echo, HIGH)>stuffyLimit);
}

// returns values from the sonar, for testing 
long readSonar(uint8_t trig, uint8_t echo){
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10);  
  digitalWrite(trig, LOW);
  return (pulseIn(echo, HIGH));
}

// Pick up something from the right
void pickUpRight(){
  swivel();
  moveClaw(susan, susanRight);
  openGrip();
  moveClaw(base, baseRightDown);
  moveClaw(elbow, elbowRightDown);
  closeGrip();
  swivel();
  dropoff();
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
}

// Pickup something from the front
void pickUpFront(){
  swivel();
  moveClaw(susan, susanFront);
  openGrip();
  moveClaw(base, baseFrontDown);
  moveClaw(elbow, elbowFrontDown);
  closeGrip();
  swivel();
  dropoff();
}

// Drops stuff off in the basket
void dropoff(){
  moveClaw(susan, susanBasket);
  moveClaw(base, baseDropoff);
  moveClaw(elbow, elbowDropoff);
  openGrip();
  travel();
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

//boolean imagineIDidAllThatAndIMissedIt(){
  //return !digitalRead(touchSensorPin);
//}
