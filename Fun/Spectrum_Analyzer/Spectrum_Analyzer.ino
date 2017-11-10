/*
 * |IRQ |MISO|
 * |MOSI|SCK |
 * |CSN | CE |
 * |VCC |GND |
 */
// Define your pins here!
#include <SPI.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
#define CE_pin 48
#define CSN_pin 49
#define MOSI_pin 51
#define MISO_pin 50
#define SCK_pin 52 
#define HIGH_LOW PORTL
byte data_in[2], channel;
byte one[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111
};
byte two[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111
};
byte three[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111
};
byte four[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111
}; 
byte five[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte six[8] = {
  0b00000,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte seven[8] = {
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte eight[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

void setup(){
  Serial.begin(250000);
  Serial.println("Setting Up");
  lcd.createChar(1,one);
  lcd.createChar(2,two);
  lcd.createChar(3,three);
  lcd.createChar(4,four);
  lcd.createChar(5,five);
  lcd.createChar(6,six);
  lcd.createChar(7,seven);
  lcd.createChar(8,eight);
  lcd.begin(16, 2);
  NRF_Init();
  NRF_bit_write(0, 0, 1);//register#, bit#, and value 0 or 1, ::  0,0,1 RX Mode
  NRF_bit_write(0, 1, 1);//register, bit, and value 0,1,1 PowerUP
  NRF_bit_write(0, 4, 1);//RT Mask turns off the RT interrupt
  NRF_bit_write(0, 5, 1);//TX Mask turns off the TX interrupt
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100010);//flush RX
  digitalWrite(CSN_pin, HIGH);
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100001);
  digitalWrite(CSN_pin, HIGH);
  NRF_ClearInterrupts();
}
int total;
void loop(){
  NRF_write_channel();
  NRF_get_channel();
  total+=data_in[1];
  if (channel%4==0){
    lcd.setCursor(((channel%64)/4), channel/64);
    if (total>8){
      total=8;
    }
    lcd.write(total);
    total=1;
  }
  if (data_in[1]>0){ 
    Serial.print(channel,BIN);
    Serial.print(" = ");
    Serial.println(data_in[0],BIN);
  }
}

void NRF_Init(){
  pinMode(CE_pin, OUTPUT);//chip enable set as output
  pinMode(CSN_pin, OUTPUT);//chip select pin as output
  pinMode(MOSI_pin, OUTPUT);//SPI data out
  pinMode(MISO_pin, INPUT); //SPI data in
  pinMode(SCK_pin, OUTPUT);//SPI clock out
  Serial.println("NRF Pins Initialized");
  SPI.setBitOrder(MSBFIRST);//SPI Most Significant Bit First
  SPI.setDataMode(SPI_MODE0);// Mode 0 Rising edge of data, keep clock low
  SPI.setClockDivider(SPI_CLOCK_DIV2);//Run the data in at 16MHz/2 - 8MHz
  digitalWrite(CE_pin, HIGH);//RX mode
  digitalWrite(CSN_pin, HIGH);//SPI idle
  SPI.begin();//start up the SPI library
  Serial.println("NRF Ready");
}

void NRF_ClearInterrupts(){
  NRF_get_address(7);//RT interrupt
  if(bitRead(data_in[1], 4))
  NRF_bit_write(7,4,1);
  
  NRF_get_address(7);//TX interrupt
  if(bitRead(data_in[1], 5))
  NRF_bit_write(7,5,1);
  
  NRF_get_address(7);//RX interrupt
  if(bitRead(data_in[1], 6))
  NRF_bit_write(7,6,1);
}

void NRF_bit_write(byte address, byte bit_add, byte val){
  NRF_get_address(address);
  if(val==1){
    bitSet(data_in[1],bit_add);
  }
  else{
    bitClear(data_in[1],bit_add);
  }
 
  bitClear(HIGH_LOW,0);
  data_in[0] = SPI.transfer(32+address);
  data_in[1] = SPI.transfer(data_in[1]);
  bitSet(HIGH_LOW,0);
}

void NRF_write_channel(){
  channel++;
  if (channel==128){
    channel=0;
      Serial.println("-----------------------New Round-----------------------");
  }
  bitClear(HIGH_LOW,0);
  data_in[0] = SPI.transfer(B100101);
  data_in[1] = SPI.transfer(channel);
  bitSet(HIGH_LOW,0);
  delayMicroseconds(125);
}

void NRF_get_address(byte address){
  bitClear(HIGH_LOW,0);
  data_in[0] = SPI.transfer(address);
  data_in[1] = SPI.transfer(B00000000);
  bitSet(HIGH_LOW,0);
}

void NRF_get_channel(){
  bitClear(HIGH_LOW,0);
  data_in[0] = SPI.transfer(9);
  data_in[1] = SPI.transfer(B00000000);
  bitSet(HIGH_LOW,0); 
}

  
  

