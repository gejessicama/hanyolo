 #include <inttypes.h>
#include "HardwareSerial.h"

// 12 Step Rotary Encoder with Click //
// http://www.sparkfun.com/products/9117 //
#define EncoderPinA 2  // Rotary Encoder Left Pin //
#define EncoderPinB 3  // Rotary Encoder Right Pin //

// ======================================================================================= //
class Encoder
{
  public:
    Encoder()
    {
      pinMode(EncoderPinA, INPUT);
      //digitalWrite(EncoderPinA, HIGH);
      pinMode(EncoderPinB, INPUT);
      //digitalWrite(EncoderPinB, HIGH);

      Position = 0;
      Position2 = 0;
      Max = 127;
      Min = 0;
      clickMultiply = 10;
    }

    void Tick(void)
    {
      Position2 = (digitalRead(EncoderPinB) * 2) + digitalRead(EncoderPinA);;
      if (Position2 != Position)
      {
        isFwd = ((Position == 0) && (Position2 == 1)) || ((Position == 1) && (Position2 == 3)) ||
                ((Position == 3) && (Position2 == 2)) || ((Position == 2) && (Position2 == 0));
        if (isFwd) Pos++;
        else Pos--;
        if (Pos < Min) Pos = Min;
        if (Pos > Max) Pos = Max;
      }
      Position = Position2;
    }

    int getPos(void)
    {
      return (Pos / 4);
    }

    void setMinMax(int _Min, int _Max)
    {
      Min = _Min * 4;
      Max = _Max * 4;
      if (Pos < Min) Pos = Min;
      if (Pos > Max) Pos = Max;
    }

    void setClickMultiply(int _clickMultiply)
    {
      clickMultiply = _clickMultiply;
    }

  private:
    int clickMultiply;
    volatile int Max;
    volatile int Min;
    volatile int Pos;
    volatile int Position;
    volatile int Position2;
    volatile int isFwd;
};
