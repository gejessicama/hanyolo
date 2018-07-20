#include "INCLUDES.h"

void setup(){
  LCD.begin();
  Serial.begin(9600);
}

Motion robotMotion(54,0);

void loop() {

    while(true) {
      robotMotion.followTape();
      long pos0 = robotMotion.getEncoder0(false,true);
      long pos1 = robotMotion.getEncoder1(false,true);
      LCD.clear();
      LCD.print("P0 ");
      LCD.print(pos0);
      LCD.print("\t");
      LCD.print("P1 ");
      LCD.print(pos1);
      /*
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


