#include <Arduino.h>
#ifndef CLAW_H
#define CLAW_H

class Claw{
  private:
    int objectLimit;
    
  public:
    Claw(uint16_t);
    boolean pickUpRight();
    boolean pickUpLeft();
    boolean pickUpFront();
  
};

#endif

