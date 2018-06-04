int p = 0;
void setup() {
 pinMode(p,OUTPUT);
}

void loop() {
digitalWrite(p, HIGH);
delay(1000);
digitalWrite(p, LOW);
delay(1000);
}
