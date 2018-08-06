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

#define toSoloPin 1
#define fromSoloRightPin 2
#define fromSoloLeftPin 3

#define moveDelay 500


#define susanRight 43
#define susanFront 0
#define susanLeft 142
#define susanBasket 100
#define susanTravelRight 100
#define susanTravelLeft 142

#define baseRightDown 100
#define elbowRightDown 180

#define baseLeftDown 100
#define elbowLeftDown 180

#define baseDropoff 180
#define elbowDropoff 0

#define baseTravel 180
#define elbowTravel 80

#define baseSwivel 180
#define elbowSwivel 0

#define gripOpen 180
#define gripClose 0

const int objectLimit = 250;
const int readWait = 800; //In microseconds

int onValue, offValue;
Servo susan, base, elbow, grip;
boolean readInQSD(uint8_t, uint8_t);
void pickUpRight();
void pickUpLeft();
void travelRight();
void travelLeft();
void dropoff();
void swivel();
void startClaw();
void endClaw();
void moveClaw(Servo, uint8_t);

void setup() {
  pinMode(rightLEDPin, OUTPUT);
  pinMode(leftLEDPin, OUTPUT);
  pinMode(toSoloPin, OUTPUT);
}

void loop() {
  if (digitalRead(fromSoloRightPin) == HIGH && readInQSD(rightLEDPin, rightQSDPin)) {
    digitalWrite(toSoloPin, HIGH);
    delay(3000);
    digitalWrite(toSoloPin, LOW);
  }

  if (digitalRead(fromSoloLeftPin) == HIGH && readInQSD(leftLEDPin, leftQSDPin)) {
    digitalWrite(toSoloPin, HIGH);
    delay(3000);
    digitalWrite(toSoloPin, LOW);
  }
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
void pickUpRight() {
  startClaw();
  swivel();
  moveClaw(susan, susanRight);
  moveClaw(grip, gripOpen);
  moveClaw(base, baseRightDown);
  moveClaw(elbow, elbowRightDown);
  moveClaw(grip, gripClose);
  swivel();
  dropoff();
  travelRight();
  endClaw();
}

// Pickup something from the left
void pickUpLeft() {
  startClaw();
  swivel();
  moveClaw(susan, susanLeft);
  moveClaw(grip, gripOpen);
  moveClaw(base, baseLeftDown);
  moveClaw(elbow, elbowLeftDown);
  moveClaw(grip, gripClose);
  swivel();
  dropoff();
  travelLeft();
  endClaw();
}

// Drops stuff off in the basket
void dropoff() {
  moveClaw(susan, susanBasket);
  moveClaw(base, baseDropoff);
  moveClaw(elbow, elbowDropoff);
  moveClaw(grip, gripOpen);
}

// The position for the claw to be in while we're just running the course
void travelRight() {
  moveClaw(susan, susanTravelRight);
  moveClaw(base, baseTravel);
  moveClaw(elbow, elbowTravel);
  moveClaw(grip, gripClose);
}

void travelLeft() {
  moveClaw(susan, susanTravelLeft);
  moveClaw(base, baseTravel);
  moveClaw(elbow, elbowTravel);
  moveClaw(grip, gripClose);
}

// Position before susan moves so that we don't hit anything
void swivel() {
  moveClaw(base, baseSwivel);
  moveClaw(elbow, elbowSwivel);
  moveClaw(grip, gripClose);
}

// Moves a servo to a given position, with a delay to allow for smoother movement
void moveClaw(Servo servo, uint8_t pos) {
  servo.write(pos);
  delay(moveDelay);
}

// Attaches all servos, right before movement for power saving
void startClaw() {
  base.attach(clawBasePin);
  elbow.attach(clawElbowPin);
  grip.attach(clawGripPin);
  susan.attach(clawSusanPin);
}

// Detaches all servos, after movement, for power saving
void endClaw() {
  base.detach();
  elbow.detach();
  grip.detach();
  susan.detach();
}
