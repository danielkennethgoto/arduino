#include <VarSpeedServo.h>


VarSpeedServo servo1;
VarSpeedServo servo2;
VarSpeedServo servo3;
void setup() {
  // put your setup code here, to run once:
servo1.attach(3);
servo2.attach(5);
servo3.attach(6);
}

void loop() {
  servo3.write(90,100);
  servo1.write(0,100);
  servo2.write(180,100);
  delay(10000);
  servo1.write(180,100);
  servo2.write(0,100);
  delay(1000);
  
  
}
