#include<Arduino.h>
#include<phys253.h>
#ifndef MOTION_H
#define MOTION_H

class Motion {
  private:
    uint8_t rightMotor, leftMotor;

    byte baseSpeed, proportionalGain, derivativeGain;
    double powerMult;
    int onTape, overCliff, backupTime;
    
    int currentError, lastError, lastState, lastOn;
    int rVal, lVal;
    int proportionalTerm, derivativeTerm, gain;
    uint16_t count;
    
    boolean isOnWhite(uint8_t);
    boolean isOverCliff(uint8_t);
    void pidControl();

  public:
    Motion(uint8_t, uint8_t);
    void setConstants();
    void followTape(uint8_t, uint8_t);
    void followRightEdge(uint8_t, uint8_t);

    void reset();
    void driveMotors(int);

};

#endif
