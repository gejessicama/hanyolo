#include "INCLUDES.h"

Motion robot(54,0);
void setup(){
  LCD.begin();
  Serial.begin(9600);
  robot.ResetServo();
}

void loop() {
    robot.followTape();
    robot.cliff();
    
}


