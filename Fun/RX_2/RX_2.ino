#include <SPI.h>

#define CE_pin 4
#define CSN_pin 5
#define IRQ_pin 2
#define MOSI_pin 11
#define MISO_pin 12
#define SCK_pin 13 

byte data_in[5], data2, data3;

void setup(){
  Serial.begin(115200);
  Serial.println("Setting Up");
  delay(100);
  NRF_Init();//set up the SPI and define pins
  NRF_set_RX_payload(0, 3);//pipe 0-5, bytes 1-32
  NRF_get_address(7, 1);//Get Reg7 Status, 1=print screen
  NRFwrite_bit_write(0, 0, 1);//register#, bit#, and value 0 or 1, ::  0,0,1 RX Mode
  NRFwrite_bit_write(0, 1, 1);//register, bit, and value 0,1,1 PowerUP
  NRFwrite_bit_write(0, 4, 1);//RT Mask turns off the RT interrupt
  NRFwrite_bit_write(0, 5, 1);//TX Mask turns off the TX interrupt
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100010);//flush RX
  digitalWrite(CSN_pin, HIGH);
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100001);//flush TX
  digitalWrite(CSN_pin, HIGH);
  
  NRF_ClearInterrupts();//clears any interrupts
  delay(100);
  attachInterrupt(digitalPinToInterrupt(IRQ_pin), get_data, FALLING);
}

void loop(){
  }
  
void get_data(){
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B01100001);//read the payload
  data_in[1] = SPI.transfer(B00000000);
  data_in[2] = SPI.transfer(B00000000);
  data_in[3] = SPI.transfer(B00000000);//action digital Read
  digitalWrite(CSN_pin, HIGH);
  Serial.println(data_in[2]);
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100010);//flush RX
  digitalWrite(CSN_pin, HIGH);
  NRFwrite_bit_write(7,6,1);//clear the RX interrupt flag
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

void NRF_set_RX_payload(byte pipe, byte bytes){
  byte address=pipe+32+16+1;// a register write starts at 32, so add on the 1 and 16 to get you to at R17
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(address);//write register 11 RX_PW_P0
  data_in[1] = SPI.transfer(bytes);//3 bytes for now
  digitalWrite(CSN_pin, HIGH);
  Serial.print("RX Payload Set RX_PW_P");
  Serial.print(pipe);
  Serial.print(" for ");
  Serial.print(bytes);
  Serial.println(" bytes");
}

void NRFwrite_bit_write(byte address, byte bit_add, byte val){
  NRF_get_address(address, 0);//first read out the register
  if(val==1)//if we want to write a one to the bit then set the bit in the register we read
  bitSet(data_in[1],bit_add);
  else
  bitClear(data_in[1],bit_add);//clear it if not
  
  digitalWrite(CSN_pin, LOW);//now we'll write the modified data back in
  data_in[0] = SPI.transfer(32+address);//a write to a register adds 32
  data_in[1] = SPI.transfer(data_in[1]);//write the modified register
  digitalWrite(CSN_pin, HIGH);
}

void NRF_ClearInterrupts(){
  NRF_get_address(7, 0);//RT interrupt
  if(bitRead(data_in[1], 4))
  NRFwrite_bit_write(7,4,1);
  
  NRF_get_address(7, 0);//TX interrupt
  if(bitRead(data_in[1], 5))
  NRFwrite_bit_write(7,5,1);
  
  NRF_get_address(7, 0);//RX interrupt
  if(bitRead(data_in[1], 6))
  NRFwrite_bit_write(7,6,1);

}

void NRF_get_address(byte address, byte info){
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(address);
  data_in[1] = SPI.transfer(B00000000);
  digitalWrite(CSN_pin, HIGH);
  if (info==1){
    Serial.print(address);
    Serial.print(" = ");
    Serial.println(data_in[1]);
  }
}
