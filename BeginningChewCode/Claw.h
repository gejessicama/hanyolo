#include <Arduino.h>
#include <Servo.h>
#ifndef CLAW_H
#define CLAW_H

#define deLae 500
#define bigDelae 500

#define susanLeft 165
#define susanFront 20
#define susanRight 60
#define susanBasket 117
#define susanTravel 117

#define baseLeftDown 80
#define baseFrontDown 70
#define baseRightDown 100
#define baseDropoff 0
#define baseTravel 0
#define baseSwivel 0

#define elbowLeftDown 180
#define elbowFrontDown 150
#define elbowRightDown 180
#define elbowDropoff 0
#define elbowTravel 95
#define elbowSwivel 0

#define gripOpen 120
#define gripClose 0

class Claw {
  private:
    Servo susan, base, elbow, grip;
    //uint8_t susanPin, basePin, elbowPin, gripPin;

  public:
    Claw(uint8_t, uint8_t, uint8_t, uint8_t);
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

