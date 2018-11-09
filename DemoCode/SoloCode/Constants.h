/*
 * A list of pin numbers and constants used
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define rightMotor 0
#define leftMotor 1

#define rightMostQRD 0
#define rightMiddleQRD 1
#define leftMiddleQRD 2
#define leftMostQRD 3
#define rightOutQRD 4
#define rightInQRD 5

#define fromChewPin 6
#define toChewPinLeft 8
#define toChewPinRight 9
#define irSignalPin 1


#define baseDrivingSpeed 153
#define pGainTapeFollowing 20
#define dGainTapeFollowing 12
#define pGainEdgeFollowing 25
#define dGainEdgeFollowing 12

const uint8_t firstBridgeServoAngle = 90;
const uint8_t secondBridgeServoAngle = 170;
const uint16_t bridgeDropWaitTime = 1500;
const uint16_t ewokFindTapeTime = 2100;
const uint16_t findTapeWaitTime = 1000;
const uint16_t lostAndFoundTime = 1700;
const uint16_t waitForClaw = 5000;

void saveMenuValues();
void displayQRDVals();

#endif
