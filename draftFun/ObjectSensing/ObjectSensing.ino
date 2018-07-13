
/*
 * Code for using the Sonars to detect objects and cliffs
 */

#define trigPinR 8
#define echoPinR 9
#define trigPinL 10
#define echoPinL  11
#define objectLim 400
#define cliffLim 1000
#define loopLen 5

uint16_t durationR, durationL;
uint8_t countObjectR, countObjectL, countCliffR, countCliffL;
//byte commVal;
 
void setup(){
    Serial.begin(9600); 
    pinMode(trigPinR, OUTPUT); 
    pinMode(trigPinL, OUTPUT);  
}

void loop(){
  countObjectR = 0, countObjectL = 0, countCliffR = 0, countCliffL = 0;
  
  for (int i=0; i<loopLen; i++){
    durationR = sonarRead(trigPinR, echoPinR);
    durationL = sonarRead(trigPinL, echoPinL);
    if (durationR < objectLim)
      countObjectR++;
    if (durationR > cliffLim)
      countCliffR++;
    if (durationL < objectLim)
      countObjectL++;
    if (durationL > cliffLim)
      countCliffL++;
  }


  if (countObjectR > loopLen/2)
    Serial.write('s');
  if (countObjectL > loopLen/2)
    Serial.write('S');
  if (countCliffR > loopLen/2)
    Serial.write('c');
  if (countCliffL > loopLen/2)
    Serial.write('C');
}

/*
 * givens: trigger and echo pins for a sonar
 * returns: duration of pulse on the echo pin
 * requires: trig is LOW when the function is called
 */
 
uint16_t sonarRead(uint8_t trig, uint8_t echo){
    digitalWrite(trig, HIGH); 
    delayMicroseconds(10);  
    digitalWrite(trig, LOW);
    return pulseIn(echo, HIGH);
}

