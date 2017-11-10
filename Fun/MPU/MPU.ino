#include <I2Cdev.h>
#include "MPU6050.h"
#include "Wire.h"
int16_t ax, ay, az;
byte x, y, sign;
MPU6050 MPU;
void setup() {
  Wire.begin();
  Serial.begin(15200);
  MPU.initialize();

}

void loop() {
  // put your main code here, to run repeatedly:
  MPU.getAcceleration(ax,ay,az);
  Serial.println(ax);
}
