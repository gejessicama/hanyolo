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

const int objectLimit = 650;
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
    //Serial.println("Seen on right");
    digitalWrite(toSoloPinRight, HIGH);
    //digitalWrite(toSoloPinRight, LOW);
    delay(pauseForStuffy);
    //digitalWrite(toSoloPinRight, HIGH);
    digitalWrite(toSoloPinRight, LOW);
  }
  if (readInQSD(leftLEDPin, leftQSDPin)) {
    //Serial.println("Seen on left");
    digitalWrite(toSoloPinLeft, HIGH);
    //digitalWrite(toSoloPinLeft, LOW);
    delay(pauseForStuffy);
    //digitalWrite(toSoloPinLeft, HIGH);
    digitalWrite(toSoloPinLeft, LOW);
  }
  //delay(500);
}

boolean readInQSD(uint8_t ledPin, uint8_t qsdPin) {
  digitalWrite(ledPin, HIGH);
  onValue = analogRead(qsdPin);
  delay(20);
  digitalWrite(ledPin, LOW);
  offValue = analogRead(qsdPin);
  return ((onValue - offValue) > objectLimit);
}
