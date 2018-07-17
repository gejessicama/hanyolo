#include "INCLUDES.h"

void setup(){
  LCD.begin();
  Serial.begin(9600);
}

Motion robot(54,0);

void loop() {
    robot.followTape();
    robot.cliff();
    
}


