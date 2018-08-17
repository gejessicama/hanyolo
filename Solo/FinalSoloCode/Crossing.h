/*
   Crossing is a class with objects intended to contain all functions related to
   detecting a cliff, droping a bridge, and detecting the IR signal after the gap
*/


#include <Arduino.h>
#include <phys253.h>
#include <EEPROM.h>
#ifndef CROSSING_H
#define CROSSING_H

class Crossing {
  private:
    uint16_t overCliff, backupTime;
    double backupPowerMult, rightWheelPercent;

  public:
    Crossing(int);
    boolean cliff();
    void dropBridge();
    bool detect10KIR();

};

#endif
