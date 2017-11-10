

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2C_dev_Goto.h"
#include "MPU6050.h"
#include "Wire.h"
#define COUNT 10
#define MAX_G_OFFSET 10000
#define MAX_A_OFFSET 50000
#define CALIBRATION_TIME 30000
#define CNT_MIN 5
#define P_scalar 10
#define OUTPUT_READABLE_ACCELGYRO
//#define OUTPUT_BINARY_ACCELGYRO

MPU6050 accelgyro;
double  averageax[COUNT] = {};
double  averageay[COUNT] = {};
double  averageaz[COUNT] = {};
double  averagegx[COUNT] = {};
double  averagegy[COUNT] = {};
double  averagegz[COUNT] = {};
int16_t ax,           ay,           az,           gx,           gy,           gz;
int     sumax = 0,    sumay = 0,    sumaz = 0,    sumgx = 0,    sumgy = 0,    sumgz = 0;
bool    ax_cal = 0,   ay_cal = 0,   az_cal = 0,   gx_cal = 0,   gy_cal = 0,   gz_cal = 0;
double  offsetax = 0, offsetay = 0, offsetaz = 0, offsetgx = 0, offsetgy = 0, offsetgz = 0;
int     ax_0_cnt = 0, ay_0_cnt = 0, az_0_cnt = 0, gx_0_cnt = 0, gy_0_cnt = 0, gz_0_cnt = 0;

int     index = 0;
bool    calibrated = false;





void setup() {
  Wire.begin();
  Serial.begin(250000);
  accelgyro.initialize();
  calibrate_ag();
}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  averageax[index] = ax;
  averageay[index] = ay;
  averageaz[index] = az;
  averagegx[index] = gx;
  averagegy[index] = gy;
  averagegz[index] = gz;
      
  sumax = 0;
  for (int i = 0; i<COUNT; i++){
    sumax+=averageax[i];} 
  sumay = 0;
  for (int i = 0; i<COUNT; i++){
    sumay+=averageay[i];}
  sumaz = 0;
  for (int i = 0; i<COUNT; i++){
    sumaz+=averageaz[i];}
  sumgx = 0;
  for (int i = 0; i<COUNT; i++){
    sumgx+=averagegx[i];}
  sumgy = 0;
  for (int i = 0; i<COUNT; i++){
    sumgy+=averagegy[i];}
  sumgz = 0;
  for (int i = 0; i<COUNT; i++){
    sumgz+=averagegz[i];}
    
        Serial.print("Calibrated: True ");
        Serial.print(" a/g:\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.print(gz); Serial.print("\t");
        Serial.print(offsetax/100);Serial.print("\t");
        Serial.print(offsetay/100);Serial.print("\t");
        Serial.print(offsetaz/100);Serial.print("\t");
        Serial.print(offsetgx/100);Serial.print("\t");
        Serial.print(offsetgy/100);Serial.print("\t");
        Serial.print(offsetgz/100);Serial.print("\t");
        Serial.println(millis());
}

void calibrate_ag(void){
  while (!calibrated){
        accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        averageax[index] = ax;
        averageay[index] = ay;
        averageaz[index] = az;
        averagegx[index] = gx;
        averagegy[index] = gy;
        averagegz[index] = gz;
        
        sumax = 0;
        for (int i = 0; i<COUNT; i++){
          sumax+=averageax[i];} 
          
        sumay = 0;
        for (int i = 0; i<COUNT; i++){
          sumay+=averageay[i];}
          
        sumaz = 0;
        for (int i = 0; i<COUNT; i++){
          sumaz+=averageaz[i];}
          
        sumgx = 0;
        for (int i = 0; i<COUNT; i++){
          sumgx+=averagegx[i];}
          
        sumgy = 0;
        for (int i = 0; i<COUNT; i++){
          sumgy+=averagegy[i];}
          
        sumgz = 0;
        for (int i = 0; i<COUNT; i++){
          sumgz+=averagegz[i];}
        
        Serial.print("Calibrated: False");
        Serial.print(" a/g:\t");
        Serial.print(sumax/COUNT); Serial.print("\t");
        Serial.print(sumay/COUNT); Serial.print("\t");
        Serial.print(sumaz/COUNT); Serial.print("\t");
        Serial.print(sumgx/COUNT); Serial.print("\t");
        Serial.print(sumgy/COUNT); Serial.print("\t");
        Serial.print(sumgz/COUNT); Serial.print("\t");
        Serial.print(offsetax/100);Serial.print("\t");
        Serial.print(offsetay/100);Serial.print("\t");
        Serial.print(offsetaz/100);Serial.print("\t");
        Serial.print(offsetgx/100);Serial.print("\t");
        Serial.print(offsetgy/100);Serial.print("\t");
        Serial.print(offsetgz/100);Serial.print("\t");
        Serial.println(millis());
        if (sumax/COUNT==0 and ax_cal==0){
          ax_0_cnt+=1;
        }
        if (sumay/COUNT==0 and ay_cal==0){
          ay_0_cnt+=1;
        }
        if (sumaz/COUNT==0 and az_cal==0){
          az_0_cnt+=1;
        }
        if (sumgx/COUNT==0 and gx_cal==0){
          gx_0_cnt+=1;
        }
        if (sumgy/COUNT==0 and gy_cal==0){
          gy_0_cnt+=1;
        }
        if (sumgz/COUNT==0 and gz_cal==0){
          gz_0_cnt+=1;
        }

        if (ax_0_cnt>=CNT_MIN){
          ax_cal==true;
        }
        if (ay_0_cnt>=CNT_MIN){
          ay_cal==true;
        }
        if (az_0_cnt>=CNT_MIN){
          az_cal==true;
        }
        if (gx_0_cnt>=CNT_MIN){
          gx_cal==true;
        }
        if (gy_0_cnt>=CNT_MIN){
          gy_cal==true;
        }
        if (gz_0_cnt>=CNT_MIN){
          gz_cal==true;
        }
        

        index +=1;
        
        if (index==COUNT){
          index = 0;}
          
        if (ax_cal==false){
          offsetax-=(sumax/COUNT)/P_scalar;
          if (offsetax>MAX_A_OFFSET){
            offsetax = MAX_A_OFFSET;}
          else if(offsetax<-MAX_A_OFFSET){
            offsetax = -MAX_A_OFFSET;}
        }
        
        if (ay_cal==false){
          offsetay-=(sumay/COUNT)/P_scalar;
          if (offsetay>MAX_A_OFFSET){
            offsetay = MAX_A_OFFSET;}
          else if(offsetay<-MAX_A_OFFSET){
            offsetay = -MAX_A_OFFSET;}
        }

        if (az_cal==false){
          offsetaz-=(sumaz/COUNT/P_scalar);
          if (offsetaz>MAX_A_OFFSET){
            offsetaz = MAX_A_OFFSET;}
          else if(offsetaz<-MAX_A_OFFSET){
            offsetaz = -MAX_A_OFFSET;}
        }

        if (gx_cal==false){
          offsetgx-=(sumgx/COUNT)/P_scalar;
          if (offsetgx>MAX_G_OFFSET){
            offsetgx = MAX_G_OFFSET;}
          else if(offsetgx<-MAX_G_OFFSET){
            offsetgx = -MAX_G_OFFSET;}
        }

        if (gy_cal==false){
          offsetgy-=(sumgy/COUNT)/P_scalar;
          if (offsetgy>MAX_G_OFFSET){
            offsetgy = MAX_G_OFFSET;}
          else if(offsetgy<-MAX_G_OFFSET){
            offsetgy = -MAX_G_OFFSET;}
        }
        
        if (gz_cal==false){
        offsetgz-=(sumgz/COUNT)/P_scalar;
        if (offsetgz>MAX_G_OFFSET){
          offsetgz = MAX_G_OFFSET;}
        else if(offsetgz<-MAX_G_OFFSET){
          offsetgz = -MAX_G_OFFSET;}
        }

        accelgyro.setXAccelOffset(0);
        accelgyro.setYAccelOffset(offsetay/100);
        accelgyro.setZAccelOffset(offsetaz/100);
        accelgyro.setXGyroOffset(offsetgx/100);
        accelgyro.setYGyroOffset(offsetgy/100);
        accelgyro.setZGyroOffset(offsetgz/100);
        if (millis()>CALIBRATION_TIME){
          calibrated=true;}
  }
}

