/*
 * Function draft for Cliff Following on the TINAH
 */

#include <phys253.h>

#define motorPinR 0
#define motorPinL 1

const int ON = 100;


void setup() {
  #include <phys253setup.txt>
}

void loop() {
  // put your main code here, to run repeatedly:

}

/*
 * givens: a qrd pin to read
 * returns: if the QRD is on or off the surface
 */
boolean isON (uint8_t qrdPin){
  if (analogRead(qrdPin) < ON)
    return true;
  return false;
}

int getCliffError(uint8_t rightQRD, uint8_t leftQRD){
  bool rVal = isON(rightQRD); // want this false
  bool lVal = isON(leftQRD); // want this true
  
  if (!rVal && lVal)
    return 0;
  if (!rVal && !lVal)
    return 1;
  if (rVal && lVal)
    return -1;

    // this last case of both on will be important to pay attention
    // to when we reach the end of the bridge

  // we will never see the case where right is on, but left is off,
  // unless we have already falle off a cliff
}

