#include <phys253.h>
#define IRsig 4

void setup() {
  LCD.begin();
  Serial.begin(9600);
  pinMode(IRsig, INPUT);
}
uint8_t lval;
uint8_t servoAngle1 = 90;
uint8_t servoAngle2 = 180;

void loop() {

//digitalWrite(8, HIGH);
//digitalWrite(9, LOW);
//digitalWrite(10, HIGH);
//digitalWrite(11, LOW);
//digitalWrite(12, HIGH);
//digitalWrite(13, LOW);
//digitalWrite(14, HIGH);
//digitalWrite(15, LOW);
//
//LCD.print("8 H, 15 L");
//LCD.clear();
//*****************************
//IR
//  
//  uint8_t val = digitalRead(IRsig);
//  LCD.clear();
//  LCD.println("signal");
//  //LCD.print(val);
//  if (val == HIGH){
//    LCD.print("1K");
//  }else if(val == LOW){
//    LCD.println("10K");
//  }
//*****************************


//*****************************
//Servo
//    int val = knob(6)/1024.0 *360;
//    if(val > 170) val=170;
//    RCServo0.write(val);
//    LCD.clear();
//    LCD.print(val);
    

    int val = knob(6)/1024.0 *180;
    RCServo0.write(val);
    LCD.clear();
    LCD.print(val);
    

//  for (int pos = servoAngle1 - 10; pos <= servoAngle1; pos++) {
//    RCServo0.write(pos);
//    delay(10);
//  }
//
//  delay(2000);
//
//  for (int pos = servoAngle2 - 10; pos <= servoAngle2; pos--) {
//    RCServo0.write(pos);
//    delay(10);
//  }
//*****************************

//*****************************
//Wheels
//  LCD.clear();
//  LCD.println("run");
//  int sp1 = knob(7)/1024.0 *255*2-255;
//  int sp2 = -sp1;
//  int sp3 = knob(6)/1024.0 *255*2-255;
//  motor.speed(0,sp1);
//  motor.speed(1,sp2);
//  motor.speed(2,sp3);
//  motor.speed(3,-sp3);
//  LCD.print("sp1");
//  LCD.print(sp1);
//  LCD.print(" sp3 ");
//  LCD.print(sp3);

//  
//*****************************

}
