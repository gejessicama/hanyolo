/* Read Quadrature Encoder
  Connect Encoder to Pins encoder0PinA, encoder0PinB, and +5V.

  Sketch by max wolf / www.meso.net
  v. 0.1 - very basic functions - mw 20061220

*/
/*
#include "encoderDB.h"
volatile Encoder ec;
volatile int x = 0;

void iiii () {
  ec.Tick();
  ++x;
}
 
void setup(){
  Serial.begin(9600);
}

void loop(){
  //ec.Tick();
  attachInterrupt(digitalPinToInterrupt(3),iiii,CHANGE);
  while(true) { 
  int po = ec.getPos();
  Serial.print(x);
  Serial.print("\t");
  Serial.print(digitalRead(3));
  Serial.print("\t");
  Serial.println(po);
  delay(10);
  }
  
}
*/
int encoder0PinA = 3;
int encoder0PinB = 4;

int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;

void setup() {
  pinMode (encoder0PinA, INPUT);
  pinMode (encoder0PinB, INPUT);
  Serial.begin (9600);
}
unsigned long it = millis();
unsigned long time_[8];
long pos[8];
int i = 0;

void loop() {
  //unsigned long ct = millis();
  long lastv = 0;
  while(true){
    //long p = getST(false,true);
    long vel = getST(true,false);
    if (lastv != vel)
    Serial.println(vel);
    lastv = vel;
  }
}


long getST(bool vl,bool ps){ 
  long cp = encoder0Pos;
  long velo;
  long posi = cp;
  delay(1);
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      encoder0Pos--;
    } else {
      encoder0Pos++;
    }
    unsigned long ft = millis();
    //float vel = 1000.0*((float) encoder0Pos-cp)/(ft-it+.5);
    time_[i%8] = ft;
    pos[i%8] = cp;
    float vel = 1000.0*((float)cp - pos[(i+1)%8])/(ft - time_[(i+1)%8]);
    velo = vel;
    posi = cp;
    it = ft;
    i++;
  }
   //nsigned long vel = (encoder0Pos - cp)/(50
  encoder0PinALast = n;
  if (vl){
      return velo;
    }
    if(ps){
      return posi;
    }
}

