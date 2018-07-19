#include<Arduino.h>
#ifndef MOTION_H
#define MOTION_H

class Motion {
  private:
    // VARIABLES FOR ALL
    uint8_t ON, rightMotor, leftMotor, baseSpeed;

    // VARIABLES FOR TAPEFOLLOWING
    uint8_t currentError, lastError, lastState, lastOn;
    int rVal, lVal;
    int proportionalTerm, derivativeTerm, gain;
    uint16_t count;
    
    boolean isOnWhite(uint8_t);

  public:
    Motion(uint8_t rMotor, uint8_t lMotor, uint8_t onTape, int v0);
    void followTape(uint8_t rightQRD, uint8_t leftQRD, uint8_t proportionalGain, uint8_t derivativeGain);
    void followEdge(uint8_t outQRD, uint8_t inQRD);
    void turn90Degrees();
};

#endif
