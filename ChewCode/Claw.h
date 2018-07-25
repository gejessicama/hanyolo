#include <Arduino.h>
#ifndef CLAW_H
#define CLAW_H

class Claw{
  private:
    
    boolean dropoff();
    void open();
    void close();

  public:
    Claw();
    boolean pickUpRight();
    boolean pickUpLeft();
    boolean pickUpFront();
  
};

#endif

