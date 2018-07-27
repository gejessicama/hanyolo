/*
   Time Trial Code for Chew: excludes the claw.
   When a stuffy is seen, chew writes a pin to high waits 3 seconds, writes that pin to low
*/

#include <Servo.h>

#define rightTriggerPin 4
#define rightEchoPin 5
#define leftTriggerPin 6
#define leftEchoPin  7

#define bridgeDropPin 13

#define fromSoloPin 2
#define toSoloPin 0
#define dropTheBridgePin 3
#define pauseForStuffy 3000

//const uint16_t objectLimit = 400;
const uint16_t stuffyLimit = 60000;

Servo bridge;
volatile boolean firstBridgeDropped = false;

boolean readInSonar(uint8_t, uint8_t, uint16_t);
void dropBridge();

void setup() {
  Serial.begin(9600);
  pinMode(rightTriggerPin, OUTPUT);
  pinMode(leftTriggerPin, OUTPUT);
  pinMode(toSoloPin, OUTPUT);
  bridge.attach(bridgeDropPin);
  //bridge.write(0);
  attachInterrupt(digitalPinToInterrupt(dropTheBridgePin), dropBridge, RISING);
  Serial.println("setup");
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

void dropBridge() {
  if(!firstBridgeDropped){
    bridge.write(30);
    firstBridgeDropped = true;
    Serial.println("drop first bridge");
  }else{
    bridge.write(180);
    Serial.println("drop second bridge");
  }
}


