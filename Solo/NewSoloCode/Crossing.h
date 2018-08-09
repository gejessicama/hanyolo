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
    boolean cliff();
    void dropBridge1();
    bool detect10KIR();

};

#endif
