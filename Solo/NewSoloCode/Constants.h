/*
 * CURRENT MENU VALUES AS OF 8/7/18
 * 
 * ONTAPE: 400
 * OVERCLIFF: 470
 * NORMAL POWER: 0.76, 0.83, 0.79
 * SLOW POWER: 0.44, 0.51
 * BACKUP POWER: 1.4, 1.07
 * RIGHT PERCENTAGE: 1.09, 0.74
 * FIRST BACKUP TIME: 420, 450
 * STUFFY DELAY: 21
 * RAMP POWER: 0.99
 * RETURN TIME: 3400
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define rightMotor 0
#define leftMotor 1
#define scissorLiftMotor 2

#define rightMostQRD 0
#define rightMiddleQRD 1
#define leftMiddleQRD 2
#define leftMostQRD 3
#define rightOutQRD 4
#define rightInQRD 5

#define basketSensorPin 5
#define scissorUpLimitPin 3
#define scissorDownLimitPin 4

#define fromChewPin 6
#define toChewPinLeft 8
#define toChewPinRight 9
#define irSignalPin 1


#define baseDrivingSpeed 153
//#define baseBackupSpeed 235
#define pGainTapeFollowing 20
#define dGainTapeFollowing 12
#define pGainEdgeFollowing 25
#define dGainEdgeFollowing 12

const double backPowerMult = 1.07;
const uint8_t firstBridgeServoAngle = 90;
const uint8_t secondBridgeServoAngle = 170;
const uint16_t bridgeDropWaitTime = 1500;
const uint16_t ewokFindTapeTime = 2100;
const uint16_t findTapeWaitTime = 1700;
const uint16_t lostAndFoundTime = 1700;
const uint16_t waitForClaw = 5000;


#endif
