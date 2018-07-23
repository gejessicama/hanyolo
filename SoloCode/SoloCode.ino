/*
   Code for the TINAH

   Solo

   have an interrupt for the start and stop buttons, or does the restart button take care of this?
*/

#include <phys253.h>
#include "Motion.h"
#include "Crossing.h"

#define rightMotor 0
#define leftMotor 1
#define scissorLiftMotor 2

#define rightMostQRD 1
#define rightMiddleQRD 2
#define leftMiddleQRD 3
#define leftMostQRD 4
#define rightOutQRD 5
#define rightInQRD 6

//#define basketSensorPin
//#define scissorUpLimitPin
//#define scissorDownLimitPin
//#define oneIRPin
//#define tenIRPin

#define rightEncoderPin 1 //THIS PIN MAY NOT BE WORKING
#define leftEncoderPin 2

#define fromChewPin 3
#define toChewPin 8
#define dropTheBridgePin 9


#define baseSpeed 255
//#define powerMultiplier 1
#define onTheTape 400
#define overTheCliff 700

#define pGainConst 54
#define dGainConst 0

#define distanceToStormtroopers 36
#define backUpBridgeDistance 2

volatile uint8_t state = 0;
volatile uint8_t rememberState;
volatile uint16_t rightWheelDist, leftWheelDist;

Motion hanMovo(rightMotor, leftMotor, onTheTape, overTheCliff, baseSpeed);
Crossing hanFlyo(rightMotor, leftMotor, rightMostQRD, leftMostQRD, overTheCliff, backUpBridgeDistance);

//  HELPER FUNCTIONS
void updateChewState();
void raiseBasket();
void lowerBasket();

//  INTERRUPT FUNCTIONS
void changeState();
void incrementRightPos(); //just need to know how much distance corresponds to
void incrementLeftPos(); //need to make sure wheels only turn forwards

void setup() {
  pinMode(rightEncoderPin, INPUT);
  pinMode(leftEncoderPin, INPUT);
  pinMode(fromChewPin, INPUT);
  attachInterrupt(fromChewPin, changeState, CHANGE);
}

void loop() {

  switch (state) {

    case 0 : // START BUTTON NOT YET PRESSED
      if (startbutton()) {
        updateState();
      }
      break;

    case 1 : // STARTING STATE UNTIL FIRST GAP :: could also read in QRDs to detect cliffs on the side of the robot
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);
      if (hanFlyo.cliff()) {
        updateState();
        hanFlyo.dropBridge1(dropTheBridgePin); // dropping the first bridge will include backing up to the right distance
      }
      break;

    case 2 : //CROSSING THE FIRST GAP :: could have updated tape function to wiggle if we lose the tape, side cliff detection

      // THIS NEEDS TO BE MORE THOUGH OUT
      /*
         Second bridge should block IR signal until we have turned a bit (and hopefully picked up the stuffy
         as soon as we detect the IR signal we need to stop and wait for it to change frequencies
         likely we should have a helper function we call that just loops and waits for signal to change
         and when the signal changes then we exit and increment
      */
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);

      //look for IR beacon
      //when the IR beacon changes from 1k to 10k
      updateState();
      rightWheelDist = 0;
      leftWheelDist = 0;
      attachInterrupt(digitalPinToInterrupt(rightEncoderPin), incrementRightPos, RISING);

      break;

    case 3 : //IR BEACON JUST CHANGED FREQUENCY :: side cliff?
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);
      if (rightWheelDist > distanceToStormtroopers) {
        updateState();
        detachInterrupt(digitalPinToInterrupt(rightEncoderPin));
      }
      break;

    case 4 : // WE ARE NOW PAST THE STORMTROOPERS
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);
      if (hanFlyo.cliff()) {
        updateState();
        //hanFlyo.backUp();
        //hanFlyo.turnRight();
        rightWheelDist = 0;
        leftWheelDist = 0;
        attachInterrupt(digitalPinToInterrupt(rightEncoderPin), incrementRightPos, RISING);
      }
      break;

    case 5 : //WE JUST LEFT THE TAPE AND TURNED TOWARDS THE SECOND GAP
      //move both wheels forward to cover the same distance
      //Side cliff detection???? maybe not

      if (hanFlyo.cliff()) {
        updateState();
        detachInterrupt(digitalPinToInterrupt(rightEncoderPin));
        hanFlyo.dropBridge2(dropTheBridgePin);
      }
      break;

    case 6 : //WE JUST DROPPED THE SECOND BRIDGE
      hanMovo.followTape(rightMiddleQRD, leftMiddleQRD, pGainConst, dGainConst);
      if (hanFlyo.cliff()) {
        updateState();
        rightWheelDist = 0;
        leftWheelDist = 0;
        attachInterrupt(digitalPinToInterrupt(rightEncoderPin), incrementRightPos, RISING);
      }
      break;

    case 7 : //WE JUST CROSSED OUR SECOND BRIDGE
      //move both wheels forward to cover the same distance
      //when we have moved forward a set distance
      updateState();
      detachInterrupt(digitalPinToInterrupt(rightEncoderPin));

      break;

    case 8 : //WE JUST TURNED TOWARDS THE SECOND TOWER
      hanMovo.followRightEdge(rightOutQRD, rightInQRD, pGainConst, dGainConst);
      //keep going until the basket touch sensor goes to zero
      //update states
      //lower the basket
      break;

    case 9 : //COMPLETED THE COURSE MAYBE
      // something about shutting off/ just chilling
      break;
  }
}

void updateState() {
  state++;
  digitalWrite(toChewPin, HIGH);
  digitalWrite(toChewPin, LOW);
}

void raiseBasket() {

}

void lowerBasket() {

}

// INTERRUPT FUNCTIONS
void changeState() {
  if (state == 0) {
    state = rememberState;
    if (state == 8) {
      raiseBasket();
    }
  } else {
    rememberState = state;
    state = 0;
  }
}

void incrementRightPos() {

}

void incrementLeftPos() {

}

