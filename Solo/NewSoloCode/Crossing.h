#include <Arduino.h>
#include <phys253.h>
#include <EEPROM.h>
#ifndef CROSSING_H
#define CROSSING_H

class Crossing{
  private:
    uint16_t overCliff, backupTime;
    double backupPowerMult, rightWheelPercent;

  public:
    Crossing(int);
    void setConstants();
    boolean cliff();
    void backUp(double);
    void turnRight();
    void dropBridge1();
    void dropBridge2(uint16_t);
    bool detect10KIR();
    void alignStep();
};

#endif
