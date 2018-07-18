#ifndef COMMUNICATION_H
#define COMMUNICATION_H

class Communication{
  private:
    char incomingByte, rightObject, leftObject, rightCliff, leftCliff;
    
  public:
    bool objectToRight;
    bool objectToLeft;
    bool cliffToRight;
    bool cliffToLeft;
    Communication(char, char, char, char);
    void activateClaw(int senderPin);
    void readIn();
    
    
};

#endif

