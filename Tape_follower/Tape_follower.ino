#include "INCLUDES.h"

Motion robot(54,0);
void setup(){
  LCD.begin();
  Serial.begin(9600);
  robot.ResetServo();
}

<<<<<<< HEAD
=======
Motion robotMotion(54,0);

>>>>>>> 243827279eb41b3cf559fbad039fa27535020f14
void loop() {

    while(true) {
      robotMotion.followTape();
      long pos0 = robotMotion.getEncoder0(false,true);
      LCD.clear();
      LCD.print(pos0);/*
      if(robotMotion.cliff()) {
        robotMotion.stopMoving();
        robotMotion.dropBridge1();
        break;
      }*/
    }
    /*
    while(true) {
      robotMotion.followTape(); // crossing gap
      //encoder.poll();
      //if(robotMotion.hasCrossedGap()) break;
    }
    while(true) {
      if(robot.lookForIR()) break;
    }*/
}


