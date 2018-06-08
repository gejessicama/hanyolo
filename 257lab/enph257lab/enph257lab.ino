  int pA1 = 0;
  int pA2 = 0;
  int pA3 = 0;
  int pA4 = 0;
  int pA5 = 0;
  int heat = 0;
  
  double t1;
  double t2;
  double t3;
  double t4;
  double t5;
  
void setup() {
  Serial.begin(9600);
  pinMode(8,INPUT);
}
int S1 = 0;
void loop() { 
  //this loop takes about 40 ms

  pA1 = 0;
  pA2 = 0;
  pA3 = 0;
  pA4 = 0;
  pA5 = 0;
  
  for (int i=0; i<5; i++){
     pA1 += analogRead(A1);
     pA2 += analogRead(A2);
     pA3 += analogRead(A3);
     pA4 += analogRead(A4);
     pA5 += analogRead(A5);
    }
 
  t1 = 0.1*(pA1/1.024)-50;
  t2 = 0.1*(pA2/1.024)-50;
  t3 = 0.1*(pA3/1.024)-50;
  t4 = 0.1*(pA4/1.024)-50;
  t5 = 0.1*(pA5/1.024)-50;

  heat = digitalRead(8);
  
  // Converts pin readings to string in csv format
  Serial.print(t1); Serial.print(","); Serial.print(t2); Serial.print(","); Serial.print(t3); Serial.print(","); Serial.print(t4); Serial.print(",");
  Serial.print(t5); Serial.print(","); Serial.print(heat); Serial.print(","); Serial.println(millis());
  //String data = String(t1) +  ',' + String(t2) +  ',' + String(t3) + ',' + String(t4) + ',' + String(t5) + ',' + String(heat) + ',' + String(millis());

  // Print data to serial in string format, to be split in python
  //Serial.println(data);
  delay(20);
}
