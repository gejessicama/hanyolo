#include "INCLUDES.h"

void setup(){
  LCD.begin();
  Serial.begin(9600);
}

Motion robotMotion(54,0);

void loop() {

    while(true) {
      robotMotion.followTape();
      long posR = robotMotion.getEncoder0(false,true);
      long posL = robotMotion.getEncoder1(false,true);
      LCD.clear(); 
      LCD.print("PR ");
      LCD.print(posR);
      LCD.print("  ");
      LCD.print("PL ");
      LCD.print(posL);
      
      if(robotMotion.cliff()) {
        //robotMotion.stopMoving();
        long fPosL = posL - 50.0;
        long fPosR = posR - 50.0;
        Serial.print("cliff reached, reverse now L = ");
        Serial.print(fPosL);
        Serial.print("  R = ");
        Serial.println(fPosR);
        while(true){
          Serial.print("reversing");
          bool reverseDone = robotMotion.reverse(fPosL,fPosR,posL,posR);
          if (reverseDone){
            //LCD.clear();
            //LCD.print("reversed");
            //delay(1000);
            break;
          }
        }
        
        //robotMotion.dropBridge1();
        
      }
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


