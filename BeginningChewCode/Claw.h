#include <Arduino.h>
#include <Servo.h>
#ifndef CLAW_H
#define CLAW_H

class Claw {
  private:
    Servo susan, base, elbow, grip;
    //uint8_t susanPin, basePin, elbowPin, gripPin;

  public:
    Claw(Servo,Servo,Servo,Servo);
    void pickUpRight();
    void pickUpLeft();
    void pickUpFront();
    void dropoff();
    void travel();
    void openGrip();
    void closeGrip();
    void swivel();
    void moveClaw(Servo, uint8_t);


};

#endif

