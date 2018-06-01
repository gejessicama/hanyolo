void setup() {
  Serial.begin(9600);
}
void loop() {

  int pA1 = 0;
  int pA2 = 0;
  int pA3 = 0;
  int pA4 = 0;
  int pA5 = 0;
  
  for (int i=0; i<5; i++){
     pA1 += analogRead(A1);
     pA2 += analogRead(A2);
     pA3 += analogRead(A3);
     pA4 += analogRead(A4);
     pA5 += analogRead(A5);
    }
 
  double t1 = 0.1*(pA1/1.024)-50;
  double t2 = 0.1*(pA2/1.024)-50;
  double t3 = 0.1*(pA3/1.024)-50;
  double t4 = 0.1*(pA4/1.024)-50;
  double t5 = 0.1*(pA5/1.024)-50;
  
  // Converts pin readings to string in csv format
  String data = String(t1) +  ',' + String(t2) +  ',' + String(t3) + ',' + String(t4) + ',' + String(t5) + ',' + String(millis());

  // Print data to serial in string format, to be split in python
  Serial.println(data);
  delay(2);
}
