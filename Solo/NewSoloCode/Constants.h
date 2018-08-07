/*
 * CURRENT MENU VALUES AS OF 8/2/18
 * 
 * BASE SPEED: 151, 142
 * POWER MULT: 0.76
 * power for approaching second gap: 0.48
 * P GAIN: 20
 * P Gain for the edge following: 25
 * D GAIN: 12
 * ON TAPE: 400
 * OVER CLIFF: 460
 * cliff in the morning sun: 50 with a cardboard box helping
 * over cliff for edge following: 650, 620
 * BACKUP TIME: 1610
 * backup time for after dropping the second bridge: 520
 * backup time to before dropping second bridge 420
 * BACKUP SPEED: 238
 * 241 for second gap
 * BACKUP RIGHT: 0.65
 * now it is set at 0.61
 * TURNING TIME: 90
 * backup cliff: 840
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

#define basketSensorPin 7
#define scissorUpLimitPin 4
#define scissorDownLimitPin 3

#define fromChewPin 6
#define toChewPinLeft 8
#define toChewPinRight 9
#define irSignalPin 4


#define baseDrivingSpeed 152
#define baseBackupSpeed 235
#define pGainTapeFollowing 20
#define dGainTapeFollowing 12
#define pGainEdgeFollowing 25
#define dGainEdgeFollowing 12

const int bridgeDropWaitTime = 1500;
const uint8_t firstBridgeServoAngle = 90;
const uint8_t secondBridgeServoAngle = 170;
const uint16_t findTapeWaitTime = 1700;

#endif
