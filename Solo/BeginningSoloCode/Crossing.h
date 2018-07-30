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
<<<<<<< HEAD
    void dropBridge(int, uint8_t);
    bool detectIR();
=======
    void dropBridge1(uint8_t);
    void dropBridge2(uint8_t);
    bool detect10KIR();
>>>>>>> 45e0488b18745bb47749d4c260ac2efad7286b28
};

#endif
