#ifndef COMMUNICATION_H
#define COMMUNICATION_H

class Communication{
  private:
    char incomingByte, rightObject, leftObject;
    
  public:
    bool objectToRight;
    bool objectToLeft;
    Communication(char, char);
    void activateClaw(int senderPin);
    void readIn();
    
    
};

#endif

//#ifndef MOTION_H
//#define MOTION_H
//
//class Motion {
//  private:
//    boolean isOnWhite(uint8_t);
//
//  public:
//    Motion(uint8_t rMotor, uint8_t lMotor, uint8_t onTape, int v0);
//    void followTape(uint8_t rightQRD, uint8_t leftQRD, uint8_t proportionalGain,
//                    uint8_t derivativeGain);
//    void followEdge(uint8_t outQRD, uint8_t inQRD);
//    void turn90Degrees();
//};

//#endif
