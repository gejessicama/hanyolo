/*
 * CURRENT MENU VALUES AS OF 8/7/18
 * 
 * ONTAPE: 400
 * OVERCLIFF: 380
 * NORMAL POWER: 0.76
 * SLOW POWER: 0.44
 * BACKUP POWER: 1.4
 * RIGHT PERCENTAGE: 1.09
 * FIRST BACKUP TIME: 420
 
 * cliff in the morning sun: 50 with a cardboard box helping
 * over cliff for edge following: 650, 620
 * BACKUP TIME: 151
 * backup time for after dropping the second bridge: 520
 * backup time to before dropping second bridge 420
 * BACKUP SPEED: 210 (4/3 of base speed)
 * 241 for second gap
 * BACKUP RIGHT: 0.45
 * now it is set at 0.61
 * TURNING TIME: 90
 * backup cliff: 840
 * 
 * for second cliff/gap at low power: backupPow = 
 * turntime = 960, backtime = 550, backup pow = 1.06
 * at full power: turntime = 840, backuptime = 550, backupPow = .88
 * 
 * turning time for under 16 V: 880
 * turning time for over 16V: 750
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


#define baseDrivingSpeed 153
//#define baseBackupSpeed 235
#define pGainTapeFollowing 20
#define dGainTapeFollowing 12
#define pGainEdgeFollowing 25
#define dGainEdgeFollowing 12

const int bridgeDropWaitTime = 1500;
const uint8_t firstBridgeServoAngle = 90;
const uint8_t secondBridgeServoAngle = 170;
const uint16_t findTapeWaitTime = 1700;

#endif
