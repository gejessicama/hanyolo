#include <Arduino.h>
#ifndef CROSSING_H
#define CROSSING_H

class Crossing{
  private:
    uint8_t rightMotor, leftMotor;
    uint8_t rightQRD, leftQRD;
    uint8_t overTheCliff;
   
  public:
    Crossing(uint8_t rMotor, uint8_t lMotor, uint8_t rQRD, uint8_t lQRD, uint8_t overCliff);
    boolean cliff();
    void dropBridge1();
    void dropBridge2();
    
};

#endif
