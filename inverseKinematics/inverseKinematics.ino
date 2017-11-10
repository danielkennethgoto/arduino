#include <math.h>

const double len1 = 5;
const double len2 = 4;

void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.println("First Test");
  
}

double lawOfCosines(double a, double b, double c){
  return acos((a*a + b*b - c*c) / (2 * a * b));
}

double distance(double x, double y){
  return sqrt(x*x + y*y);
}

double angle1(double x, double y){
  return atan2(y,x) + lawOfCosines(distance(x,y), len1, len2);
}

double angle2(double x, double y){
  return lawOfCosines(len1, len2, distance(x,y));
}

double deg(double rad){
  return rad * 180/PI;
}



