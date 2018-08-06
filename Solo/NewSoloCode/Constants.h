/*
 * CURRENT MENU VALUES AS OF 8/2/18
 * 
 * BASE SPEED: 161
 * POWER MULT: 0.76
 * P GAIN: 20
 * P Gain for the edge following: 25
 * D GAIN: 12
 * ON TAPE: 400
 * OVER CLIFF: 460
 * over cliff for edge following: 650
 * BACKUP TIME: 1610
 * backup time for after dropping the second bridge: 520
 * BACKUP SPEED: 238
 * BACKUP RIGHT: 0.47
 * TURNING TIME: 80
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
#define rightOutQRD 5
#define rightInQRD 4

#define basketSensorPin 7
#define scissorUpLimitPin 2
#define scissorDownLimitPin 3

#define fromChewPin 6
#define toChewPinLeft 8
#define toChewPinRight 9
#define irSignalPin 4


#endif
