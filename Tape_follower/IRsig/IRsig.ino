#include <phys253.h>
#define IRsig 4

void setup() {
  LCD.begin();
  Serial.begin(9600);
  pinMode(IRsig, INPUT);
}
uint8_t lval;
void loop() {
//  
//  uint8_t val = digitalRead(IRsig);
//  LCD.clear();
//  LCD.println("signal");
//  //LCD.print(val);
//  if (val == HIGH){
//    LCD.print("10K");
//  }else if(val == LOW){
//    LCD.println("1K");
//  }
  LCD.clear();
  LCD.println("run");
  int sp1 = knob(7)/1024.0 *255;
  int sp2 = -sp1;
  int sp3 = knob(6)/1024.0 *255*2-255;
  motor.speed(0,sp1);
  motor.speed(1,sp2);
  motor.speed(2,sp3);
  motor.speed(3,-sp3);
  LCD.print(sp3);
  
}
