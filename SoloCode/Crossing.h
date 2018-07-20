#include <Arduino.h>
#include <phys253.h>
#ifndef CROSSING_H
#define CROSSING_H

class Crossing{
  private:
    uint8_t rightMotor, leftMotor;
    uint8_t rightQRD, leftQRD;
    uint8_t overTheCliff;
    uint8_t backUpDistance;
   
  public:
    Crossing(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
    boolean cliff();
    void backUp(uint8_t);
    void turnRight();
    void dropBridge1(uint8_t);
    void dropBridge2(uint8_t);
    
};

#endif
