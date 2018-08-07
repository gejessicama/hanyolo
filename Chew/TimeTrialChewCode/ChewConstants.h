#ifndef CHEWCONSTANTS_H
#define CHEWCONSTANTS_H

#define moveDelay 450

#define susanRight 33
#define susanFront 0
#define susanLeft 147
#define susanBasketRight 80
#define susanBasketLeft 115 
#define susanTravelRight 100
#define susanTravelLeft 150

#define baseRightDown 80
#define elbowRightDown 180

#define baseLeftDown 80
#define elbowLeftDown 180

#define baseDropoff 180
#define elbowDropoff 10

#define baseTravel 180
#define elbowTravel 80

#define baseSwivel 180
#define elbowSwivel 0

#define gripOpen 180
#define gripClose 0

const int objectLimit = 250;
const int readWait = 800; //In microseconds
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
