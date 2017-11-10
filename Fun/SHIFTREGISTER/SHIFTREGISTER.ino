#define modes PORTB

void setup() {
  // put your setup code here, to run once:
pinMode(13,OUTPUT);
pinMode(12,INPUT);
pinMode(11,OUTPUT);
pinMode(10,OUTPUT);
}

void loop() {
  bitClear(modes,2);//digitalWrite(10,LOW);
  sendOne();
  sendZero();
  sendOne();
  sendZero();
  sendOne();
  sendZero();
  sendOne();
  sendZero();
  bitSet(modes,2);//digitalWrite(10,HIGH);
  delay(1);
  bitClear(modes,2);//digitalWrite(10,LOW);
  sendZero();
  sendOne();
  sendZero();
  sendOne();
  sendZero();
  sendOne();
  sendZero();
  sendOne();
  bitSet(modes,2);//digitalWrite(10,HIGH);
  delay(1);
  
}

void sendOne(){
 bitSet(modes,3);//digitalWrite(11,HIGH);
 bitSet(modes,5);//digitalWrite(13,HIGH);
 bitClear(modes,5);//digitalWrite(13,LOW);
}
void sendZero(){
 bitClear(modes,3);//digitalWrite(11,LOW);
 bitSet(modes,5);//digitalWrite(13,HIGH);
 bitClear(modes,5);//digitalWrite(13,LOW);
}

