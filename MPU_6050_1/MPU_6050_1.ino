

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2C_dev_Goto.h"
#include "MPU6050.h"
#include "Wire.h"


MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high
#define COUNT 20
double averagegx[COUNT] = {};
double averagegy[COUNT] = {};
double averagegz[COUNT] = {};
int sumgx = 0, sumgy = 0, sumgz = 0;
int index = 0;
int16_t ax, ay, az;
int16_t gx, gy, gz;
double offsetgx = 0, offsetgy = 0, offsetgz = 0;
bool calibrated = false;


// uncomment "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated
// list of the accel X/Y/Z and then gyro X/Y/Z values in decimal. Easy to read,
// not so easy to parse, and slow(er) over UART.
#define OUTPUT_READABLE_ACCELGYRO

// uncomment "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit
// binary, one right after the other. This is very fast (as fast as possible
// without compression or data loss), and easy to parse, but impossible to read
// for a human.
//#define OUTPUT_BINARY_ACCELGYRO




void setup() {
    Wire.begin();
    Serial.begin(250000);
    accelgyro.initialize();
    
    accelgyro.setXGyroOffset(0);
    accelgyro.setYGyroOffset(0);
    accelgyro.setZGyroOffset(0);

    // use the code below to change accel/gyro offset values
    /*
    Serial.println("Updating internal sensor offsets...");
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    accelgyro.setXGyroOffset(220);
    accelgyro.setYGyroOffset(76);
    accelgyro.setZGyroOffset(-85);
    Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
    Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
    Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
    Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
    Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
    Serial.print("\n");
    */
    // configure Arduino LED for

}

void loop() {
  while (!calibrated){
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
        averagegx[index] = gx;
        averagegy[index] = gy;
        averagegz[index] = gz;
        sumgx = 0;
        for (int i = 0; i<COUNT; i++){
          sumgx+=averagegx[i];
        }
        sumgy = 0;
        for (int i = 0; i<COUNT; i++){
          sumgy+=averagegy[i];
        }
        sumgz = 0;
        for (int i = 0; i<COUNT; i++){
          sumgz+=averagegz[i];
        }
     
        
        Serial.print("Calibrated: False");
        Serial.print(" a/g:\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(sumgx/COUNT); Serial.print("\t");
        Serial.print(sumgy/COUNT); Serial.print("\t");
        Serial.print(sumgz/COUNT);Serial.print("\t");
        Serial.print(offsetgx/200);Serial.print("\t");
        Serial.print(offsetgy/200);Serial.print("\t");
        Serial.print(offsetgz/200);Serial.print("\t");
        Serial.println(millis());
        Serial.print(offsetgz/200);

        index +=1;
        if (index==COUNT){
          index = 0;
        }
        
        offsetgx-=(sumgx/COUNT);
        if (offsetgx>40000){
          offsetgx = 40000;
        }
        else if(offsetgx<-40000){
          offsetgx = -40000;
        }
        
        offsetgy-=(sumgy/COUNT);
        if (offsetgy>40000){
          offsetgy = 10000;
        }
        else if(offsetgy<-40000){
          offsetgy = -40000;
        }

        offsetgz-=(sumgz/COUNT);
        if (offsetgz>40000){
          offsetgz = 40000;
        }
        else if(offsetgz<-40000){
          offsetgz = -40000;
        }

        
        accelgyro.setXGyroOffset(offsetgx/200);
        accelgyro.setYGyroOffset(offsetgy/200);
        accelgyro.setZGyroOffset(offsetgz/200);
        if (millis()>30000){
          calibrated=true;
        }
  }

  while (calibrated){
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        averagegx[index] = gx;
        averagegy[index] = gy;
        averagegz[index] = gz;
        sumgx = 0;
        for (int i = 0; i<COUNT; i++){
          sumgx+=averagegx[i];
        }
        sumgy = 0;
        for (int i = 0; i<COUNT; i++){
          sumgy+=averagegy[i];
        }
        sumgz = 0;
        for (int i = 0; i<COUNT; i++){
          sumgz+=averagegz[i];
        }
     
     Serial.print("Calibrated: True");
        Serial.print(" a/g:\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(sumgx/COUNT); Serial.print("\t");
        Serial.print(sumgy/COUNT); Serial.print("\t");
        Serial.print(sumgz/COUNT);Serial.print("\t");
        Serial.print(offsetgx/200);Serial.print("\t");
        Serial.print(offsetgy/200);Serial.print("\t");
        Serial.println(offsetgz/200);
    
  }
}

void calibrate_ag(void){
  while (!calibrated){
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
        averagegx[index] = gx;
        averagegy[index] = gy;
        averagegz[index] = gz;
        sumgx = 0;
        for (int i = 0; i<COUNT; i++){
          sumgx+=averagegx[i];
        }
        sumgy = 0;
        for (int i = 0; i<COUNT; i++){
          sumgy+=averagegy[i];
        }
        sumgz = 0;
        for (int i = 0; i<COUNT; i++){
          sumgz+=averagegz[i];
        }
     
        
        Serial.print("Calibrated: False");
        Serial.print(" a/g:\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(sumgx/COUNT); Serial.print("\t");
        Serial.print(sumgy/COUNT); Serial.print("\t");
        Serial.print(sumgz/COUNT);Serial.print("\t");
        Serial.print(offsetgx/200);Serial.print("\t");
        Serial.print(offsetgy/200);Serial.print("\t");
        Serial.print(offsetgz/200);Serial.print("\t");
        Serial.println(millis());
        Serial.print(offsetgz/200);

        index +=1;
        if (index==COUNT){
          index = 0;
        }
        
        offsetgx-=(sumgx/COUNT);
        if (offsetgx>40000){
          offsetgx = 40000;
        }
        else if(offsetgx<-40000){
          offsetgx = -40000;
        }
        
        offsetgy-=(sumgy/COUNT);
        if (offsetgy>40000){
          offsetgy = 10000;
        }
        else if(offsetgy<-40000){
          offsetgy = -40000;
        }

        offsetgz-=(sumgz/COUNT);
        if (offsetgz>40000){
          offsetgz = 40000;
        }
        else if(offsetgz<-40000){
          offsetgz = -40000;
        }        
        accelgyro.setXGyroOffset(offsetgx/200);
        accelgyro.setYGyroOffset(offsetgy/200);
        accelgyro.setZGyroOffset(offsetgz/200);
        if (millis()>30000){
          calibrated=true;
        }
  }

}

