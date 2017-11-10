#include "MPU6050.h"
byte triggerPin   = B00100000;
byte SDAPin       = B00010000;
byte SCLPin       = B00001000;
byte interruptPin = B00000100;
int MPU6050Address = 0x68; //hexadecimal address of the MPU6050
MPU6050 device(MPU6050Address,SDAPin, SCLPin);
void setup() {
 
}

void loop() {
  trigger();
  device.sendData(B11111111, SDAPin);
}




void trigger(void){
  PORTD |= triggerPin; //set pin 3 high but leave the rest of the pins untouched;
  PORTD ^= triggerPin; //set pin 3 low but leave the rest of the pins untouched;
}

