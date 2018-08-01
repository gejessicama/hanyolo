/*
   Time Trial Code for Chew: excludes the claw.
   When a stuffy is seen, chew writes a pin to high waits 3 seconds, writes that pin to low
*/


#define rightTriggerPin 4
#define rightEchoPin 5
#define leftTriggerPin 6
#define leftEchoPin  7

#define toSoloPin 1
#define pauseForStuffy 3000

const uint16_t stuffyLimit = 60000;
const uint16_t objectLimit = 400;
long duration;

boolean readInSonar(uint8_t, uint8_t, uint16_t);

void setup() {
  //Serial.begin(9600);
  pinMode(rightTriggerPin, OUTPUT);
  pinMode(leftTriggerPin, OUTPUT);
  pinMode(toSoloPin, OUTPUT);
}

void loop() {

  if (readInSonar(rightTriggerPin, rightEchoPin)) {
    //Serial.println("Seen on right");
    digitalWrite(toSoloPin, HIGH);
    //digitalWrite(toSoloPin, LOW);
    delay(pauseForStuffy);
    //digitalWrite(toSoloPin, HIGH);
    digitalWrite(toSoloPin, LOW);
  }
  if (readInSonar(leftTriggerPin, leftEchoPin)) {
    //Serial.println("Seen on left");
    digitalWrite(toSoloPin, HIGH);
    //digitalWrite(toSoloPin, LOW);
    delay(pauseForStuffy);
    //digitalWrite(toSoloPin, HIGH);
    digitalWrite(toSoloPin, LOW);
  }
  //delay(500);
}

boolean readInSonar(uint8_t trig, uint8_t echo) {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  //Serial.println(duration);
  return (duration > stuffyLimit || duration < objectLimit);
}
