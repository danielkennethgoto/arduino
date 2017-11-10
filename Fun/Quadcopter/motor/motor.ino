#include <Servo.h>
Servo servo;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
servo.attach(11);
}
int angle = 0;
void loop() {
  // put your main code here, to run repeatedly:
servo.write(angle);
delay(100);
if (angle>160){
  angle=100;
}
angle+=5;
Serial.println(angle);
}
