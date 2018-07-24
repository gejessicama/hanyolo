#include<Arduino.h>
#include<phys253.h>
#ifndef MOTION_H
#define MOTION_H

class Motion {
  private:
    // VARIABLES FOR ALL
    uint8_t ON, CLIFF, rightMotor, leftMotor, baseSpeed;
    double powerMultiplier;

    // VARIABLES FOR TAPE FOLLOWING, EDGE FOLLOWING
    int currentError, lastError, lastState, lastOn;
    int rVal, lVal;
    int proportionalTerm, derivativeTerm, gain;
    uint16_t count;
    
    boolean isOnWhite(uint8_t);
    boolean isOverCliff(uint8_t);
    void pidControl(uint8_t, uint8_t);

  public:
    Motion(uint8_t, uint8_t, uint8_t, uint8_t, int, double);
    void followTape(uint8_t, uint8_t, uint8_t, uint8_t);
    void followRightEdge(uint8_t, uint8_t, uint8_t, uint8_t);
    void bothWheelsForward(uint8_t, uint8_t, uint8_t, uint8_t); // pass both distances and PID to match
    void bothWheelsBackward(int, int); // pass both distances and PID to match
    boolean cliff();
    void reset();
    
    long getEncoder0(bool,bool);
    long getEncoder1(bool,bool);
    void turn90Degrees();
};

#endif
