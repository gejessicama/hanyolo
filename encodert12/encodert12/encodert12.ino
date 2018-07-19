#include "INCLUDES.h"

#define encoder1PinA 9
#define encoder1PinB 10

void setup(){
  LCD.begin();
  Serial.begin(9600);
}


int encoder0Pos = 0,encoder1Pos = 0;
int encoder0PinALast = LOW,encoder1PinALast = LOW;
int n = LOW;

unsigned long it = millis();
unsigned long time_[8];
long pos[8];
unsigned long time_1[8];
long pos1[8];
int i = 0, i1 = 0;


void loop(){
  /*
  Serial.print(digitalRead(encoder1PinA));
  Serial.print("\t");
  Serial.println(digitalRead(encoder1PinB));
 */ 
  long lastp = 0;
  while(true){
    long p = getEncoder1(false,true);
    //long vel = getST(true,false);
    if (lastp != p)
    Serial.println(p);
    lastp = p;
  }
}

long getEncoder1(bool velocity,bool displacement){ 
  long cp = encoder1Pos;
  long velo;
  long posi = cp;
  delay(1);
  n = digitalRead(encoder1PinA);
  if ((encoder1PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder1PinB) == LOW) {
      encoder1Pos--;
    } else {
      encoder1Pos++;
    }
    unsigned long ft = millis();
    //float vel = 1000.0*((float) encoder0Pos-cp)/(ft-it+.5);
    time_1[i1%8] = ft;
    pos1[i1%8] = cp;
    float vel = 1000.0*((float)cp - pos[(i1+1)%8])/(ft - time_[(i1+1)%8]);
    velo = vel;
    posi = cp;
    it = ft;
    i1++;
  }
   //nsigned long vel = (encoder0Pos - cp)/(50
  encoder1PinALast = n;
  if (velocity){
      return velo;
    }
    if(displacement){
      return posi;
    }
}


