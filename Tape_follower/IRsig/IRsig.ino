#include "INCLUDES.h"
#define IRsig 7

void setup() {
  LCD.begin();
  Serial.begin(9600);
  pinMode(IRsig, INPUT);
}
uint8_t lval;
void loop() {
  uint8_t val = digitalRead(IRsig);
  LCD.clear();
  LCD.println("signal");
  //LCD.print(val);
  if (val == HIGH){
    LCD.print("1K");
  }else if(val == LOW){
    LCD.println("10K");
  }
}
