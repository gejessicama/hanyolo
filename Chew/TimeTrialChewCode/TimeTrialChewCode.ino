/*
   Time Trial Code for Chew: excludes the claw.
   When a stuffy is seen, chew writes a pin to high waits 3 seconds, writes that pin to low
*/

#include <Servo.h>

#define rightLEDPin 4
#define rightQSDPin 1
#define leftLEDPin 6
#define leftQSDPin  2

#define clawSusanPin 8
#define clawBasePin 13
#define clawElbowPin 12
#define clawGripPin 11

#define toSoloPinRight 1
#define toSoloPinLeft 1
#define pauseForStuffy 3000

#define deLae 700
#define susanRight 160
#define baseRightDown 100
#define elbowRightDown 180 // END RIGHT
#define susanFront 108
#define baseFrontDown 70
#define elbowFrontDown 150 // END DOWN
#define susanLeft 60
#define baseLeftDown 90
#define elbowLeftDown 180 // END LEFT
#define susanBasket 20
#define baseDropoff 0
#define elbowDropoff 0 // END BASKET
#define susanTravel 20
#define baseTravel 0
#define elbowTravel 0 // END TRAVEL
#define baseSwivel 0
#define elbowSwivel 0 // END SWIVEL
#define gripOpen 120
#define gripClose 0

const int objectLimit = 300;
const int readWait = 1000;
int onValue, offValue;
Servo susan, base, elbow, grip;

//boolean readInQSD(uint8_t, uint8_t);

void setup() {
  //Serial.begin(9600);
  pinMode(rightLEDPin, OUTPUT);
  pinMode(leftLEDPin, OUTPUT);
  pinMode(toSoloPinRight, OUTPUT);
}

void loop() {
  if (readInQSD(rightLEDPin, rightQSDPin)) {
    digitalWrite(toSoloPinRight, HIGH);
    pickUpRight();
    digitalWrite(toSoloPinRight, LOW);
  }
  if (readInQSD(leftLEDPin, leftQSDPin)) {
    digitalWrite(toSoloPinLeft, HIGH);
    pickUpLeft();
    digitalWrite(toSoloPinLeft, LOW);
  }
 // delay(500);
}

boolean readInQSD(uint8_t ledPin, uint8_t qsdPin) {
  digitalWrite(ledPin, HIGH);
  delayMicroseconds(readWait);
  onValue = analogRead(qsdPin);
  digitalWrite(ledPin, LOW);
  delayMicroseconds(readWait);
  offValue = analogRead(qsdPin);
//  Serial.print(onValue);
//  Serial.print(", ");
//  Serial.println(offValue);
  return ((offValue - onValue) > objectLimit);
}

// Pick up something from the right
void pickUpRight(){
  startClaw();
  swivel();
  moveClaw(susan, susanRight);
  openGrip();
  moveClaw(base, baseRightDown);
  moveClaw(elbow, elbowRightDown);
  closeGrip();
  swivel();
  dropoff();
  endClaw();
}

// Pickup something from the left
void pickUpLeft(){
  startClaw();
  swivel();
  moveClaw(susan, susanLeft);
  openGrip();
  moveClaw(base, baseLeftDown);
  moveClaw(elbow, elbowLeftDown);
  closeGrip();
  swivel();
  dropoff();
  endClaw();
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

void startClaw(){
  susan.attach(clawSusanPin);
  base.attach(clawBasePin);
  elbow.attach(clawElbowPin);
  grip.attach(clawGripPin);
  }

void endClaw(){
  susan.detach();
  base.detach();
  elbow.detach();
  grip.detach();
  }
