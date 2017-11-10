

void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(3),right,FALLING);
  attachInterrupt(digitalPinToInterrupt(2),left,FALLING); 
}

void loop() {
 
}

void right(){
  Serial.println("Right");
  noInterrupts();
  
}
void left(){
  Serial.println("Left");
}

