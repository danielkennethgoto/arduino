#include <Mouse.h>

void setup() {
  // put your setup code here, to run once:
Mouse.begin();
  analogReference(INTERNAL);
  //Internal reference voltages of nominally 2.56V or AVCC are provided On-chip
  Mouse.begin;
}

void loop() {
  // put your main code here, to run repeatedly:
Mouse.move(1,1);
}
