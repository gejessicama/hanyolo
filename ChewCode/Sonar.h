#include <Arduino.h>
#ifndef SONAR_H
#define SONAR_H

class Sonar{
  private:
    uint16_t objectLimit;
    uint16_t getDistance(uint8_t, uint8_t);
    
  public:
    Sonar(uint16_t);
    boolean readIn(uint8_t,uint8_t);
    
    
};

#endif

