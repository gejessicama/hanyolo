#include "INCLUDES.h"

void setup(){
  LCD.begin();
  Serial.begin(9600);
}

Motion robot(54,0);

void loop() {
    robot.followTape();
    if(Serial1.available()){
      char c = Serial1.read();
      if (c == 's' || c == 'S' || c == 'c' || c == 'C')
        delay (10000);
    }
}


