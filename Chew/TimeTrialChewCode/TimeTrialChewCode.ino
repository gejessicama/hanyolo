/*
   Time Trial Code for Chew: excludes the claw.
   When a stuffy is seen, chew writes a pin to high waits 3 seconds, writes that pin to low
*/

//#include <Servo.h>

#define rightTriggerPin 4
#define rightEchoPin 5
#define leftTriggerPin 6
#define leftEchoPin  7

#define toSoloPin 0
#define pauseForStuffy 3000

const uint16_t stuffyLimit = 60000;

boolean readInSonar(uint8_t, uint8_t, uint16_t);

void setup() {
  pinMode(rightTriggerPin, OUTPUT);
  pinMode(leftTriggerPin, OUTPUT);
  pinMode(toSoloPin, OUTPUT);
}

void loop() {

  if (readInSonar(rightTriggerPin, rightEchoPin)) {
    digitalWrite(toSoloPin, HIGH);
    delay(pauseForStuffy);
    digitalWrite(toSoloPin, LOW);
  }
  if (readInSonar(leftTriggerPin, leftEchoPin)) {
    digitalWrite(toSoloPin, HIGH);
    delay(pauseForStuffy);
    digitalWrite(toSoloPin, LOW);
  }
}

boolean readInSonar(uint8_t trig, uint8_t echo) {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  return (pulseIn(echo, HIGH) > stuffyLimit);
}
