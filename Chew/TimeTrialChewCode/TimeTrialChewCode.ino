/*
   Time Trial Code for Chew: excludes the claw.
   When a stuffy is seen, chew writes a pin to high waits 3 seconds, writes that pin to low
*/


#define rightLEDPin 4
#define rightQSDPin 1
#define leftLEDPin 6
#define leftQSDPin  2

#define toSoloPinRight 1
#define toSoloPinLeft 1
#define pauseForStuffy 3000

const int objectLimit = 300;
const int readWait = 1000;
int onValue, offValue;

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
    delay(pauseForStuffy);
    digitalWrite(toSoloPinRight, LOW);
  }
  if (readInQSD(leftLEDPin, leftQSDPin)) {
    digitalWrite(toSoloPinLeft, HIGH);
    delay(pauseForStuffy);
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
