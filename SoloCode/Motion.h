#include<Arduino.h>
#include<phys253.h>
#ifndef MOTION_H
#define MOTION_H

class Motion {
  private:
    // VARIABLES FOR ALL
    uint8_t ON, CLIFF, rightMotor, leftMotor, baseSpeed;

    // VARIABLES FOR TAPE FOLLOWING, EDGE FOLLOWING
    uint8_t currentError, lastError, lastState, lastOn;
    int rVal, lVal;
    int proportionalTerm, derivativeTerm, gain;
    uint16_t count;
    
    boolean isOnWhite(uint8_t);
    boolean isOverCliff(uint8_t);

  public:
    Motion(uint8_t, uint8_t, uint8_t, uint8_t, int);
    void followTape(uint8_t, uint8_t, uint8_t, uint8_t);
    void followRightEdge(uint8_t, uint8_t,uint8_t, uint8_t);
    void bothWheelsForward(int, int);
    boolean cliff();
    
    long getEncoder0(bool,bool);
    long getEncoder1(bool,bool);
    void turn90Degrees();
};

#endif
