#define SWITCH 9
#define TEMP A0 
#define START 8

int tempVolt;
double temp;
void setup() {
 pinMode(START,OUTPUT);
 pinMode(SWITCH,OUTPUT);
 Serial.begin(9600);
}

void loop() {
digitalWrite(SWITCH, HIGH);
digitalWrite(START,LOW);

//tempVolt = analogRead(TEMP); //read the temp
Serial.println(0.1 * (5 * tempVolt/1.024) - 50);
delay(64000);
digitalWrite(SWITCH, LOW);
digitalWrite(START,HIGH);
//tempVolt = analogRead(TEMP); // read the temp
Serial.println(0.1 * (5*tempVolt/1.024) - 50);
delay(64000);
}
