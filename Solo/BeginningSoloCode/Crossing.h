#include <Arduino.h>
#include <phys253.h>
#include <EEPROM.h>
#ifndef CROSSING_H
#define CROSSING_H

class Crossing{
  private:
    uint8_t rightMotor, leftMotor;
    uint8_t rightQRD, leftQRD;
    int overCliff, backupTime;

    uint8_t IRsig;
   
  public:
    Crossing(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
    void setConstants();
    boolean cliff();
    void backUp();
    void turnRight();
    void dropBridge(int, uint8_t);
    bool detect10KIR();
};

#endif
