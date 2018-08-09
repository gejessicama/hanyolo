#ifndef CHEWCONSTANTS_H
#define CHEWCONSTANTS_H

/*
 * SUSAN: 0 TO 180 IS FROM LEFT TO RIGHT, CCW
 * BASE: 180 IS STRAIGHT UP, 0 IS PARALLEL TO FLOOR
 * ELBOW: 180 IS THE MOST BENT
 * GRIP: 0 IS CLOSED
 */

#define moveDelay 450

#define susanFront 0
#define susanLeft 5
#define susanRight 137 

#define baseRightDown 80
#define elbowRightDown 145

#define baseLeftDown 80
#define elbowLeftDown 155

#define susanBasketRight 73 // drops slightly to right of basket
#define susanBasketLeft 73 // drops slighty to left of basket
#define baseDropoff 180
#define elbowDropoff 0

#define susanTravelLeft 73
#define baseTravelLeft 170
#define elbowTravelLeft 50

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
void startClaw();
void endClaw();
void moveClaw(Servo, uint8_t);

#define rightLEDPin 4
#define rightQSDPin 1
#define leftLEDPin 6
#define leftQSDPin  2

#define clawSusanPin 8
#define clawBasePin 13
#define clawElbowPin 12
#define clawGripPin 11

#define toSoloPin 1
#define fromSoloRightPin 2
#define fromSoloLeftPin 3 

#endif
