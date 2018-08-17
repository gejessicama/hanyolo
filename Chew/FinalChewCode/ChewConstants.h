/*
   All our pin values and defined positions, and other constants
   for the use of the Sensors and the Claw
*/

#ifndef CHEWCONSTANTS_H
#define CHEWCONSTANTS_H

#define rightLEDPin 4
#define rightQSDPin 1
#define leftLEDPin 6
#define leftQSDPin  2

#define toSoloPin 1
#define fromSoloRightPin 2
#define fromSoloLeftPin 3

#define clawSusanPin 8
#define clawBasePin 13
#define clawElbowPin 12
#define clawGripPin 11


#define moveDelay 450

#define susanFront 0
#define susanLeft 5
#define susanRight 137

#define baseRightDown 80
#define elbowRightDown 145

#define baseLeftDown 80
#define elbowLeftDown 155

#define susanBasketRight 73
#define susanBasketLeft 73
#define baseDropoff 180
#define elbowDropoff 0

#define susanTravelRight 73
#define baseTravelRight 170
#define elbowTravelRight 50

#define baseSwivel 180
#define elbowHighSwivel 0
#define elbowLowSwivel 20

#define gripOpen 180
#define gripClose 0

const int readWait = 1000; //In microseconds

int onValue, offValue;

Servo susan, base, elbow, grip;

boolean readInQSD(uint8_t, uint8_t);
void pickUpRight();
void pickUpLeft();
void travelRight();
void travelLeft();
void dropoff(uint8_t);
void swivel();
void moveClaw(Servo, uint8_t);
void startClaw();
void endClaw();

#endif
