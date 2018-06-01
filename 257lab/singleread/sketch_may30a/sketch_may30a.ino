void setup() {
  Serial.begin(9600);
}

void loop() {
  
  int r1 = analogRead(A0);
  int r2 = analogRead(A0);
  int r3 = analogRead(A0);
  int r4 = analogRead(A0);
  int r5 = analogRead(A0);
  
  double avg = (r1+ r2 + r3 + r4 + r5)/1.024;
  double temp = (.1)*avg-50;
  
  // Converts pin readings to string in csv format

  // Print data to serial in string format, to be split in python
  Serial.println(temp);
  delay(20);
}

