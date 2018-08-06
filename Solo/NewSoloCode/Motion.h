#include<Arduino.h>
#include<phys253.h>
#include <EEPROM.h>
#ifndef MOTION_H
#define MOTION_H

class Motion {
  private:
    uint8_t rightMotor, leftMotor;

    byte baseSpeed, proportionalGain, derivativeGain, backUpSpeed;
    double powerMult;
    int onTape, overCliff, turningTime;
    
    int currentError, lastError, lastState, lastOn;
    uint16_t count;
    
    boolean isOnWhite(uint8_t);
    boolean isOverCliff(uint8_t);
    void pidControl();

  public:
    Motion(uint8_t, uint8_t);
    void setConstants();
    void reset(uint8_t);
    
    void followTape(uint8_t, uint8_t);
    void followTapeFour(uint8_t, uint8_t, uint8_t, uint8_t);
    void followRightEdge(uint8_t, uint8_t);
    void driveMotors(double);
    void stopMotors();
    void turnRight();

    bool findTape(uint8_t, uint8_t, unsigned int);
    bool findRightEdge(uint8_t, uint8_t, unsigned int);
};

#endif
