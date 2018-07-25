#include "INCLUDES.h"
#define IRsig 10

void setup() {
  LCD.begin();
  Serial.begin(9600);
}
uint8_t lval;
void loop() {
  uint8_t val = digitalRead(IRsig);
  if (val !=lval){
    LCD.clear();
    if (val == HIGH){
      LCD.print("Not 10K");
    }else if(val == LOW){
      LCD.println("10K");
    }
  }
  lval = val;
}
