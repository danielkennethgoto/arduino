//NRF      ARDUINO
//1 GND    GND
//2 VCC    3.3V
//3 CE     4  out
//4 CSN    5  out
//5 SCK    13 out
//6 MOSI   11 out
//7 MISO   12 in
//8 IRQ    2  in

#include <SPI.h>
#define CE_pin 4
#define CSN_pin 5
#define IRQ_pin 2
#define MOSI_pin 11
#define MISO_pin 12
#define SCK_pin 13 
#define packet_size 1 //number of bytes getting sent each time
#define baud_rate 115200

byte data_in[packet_size+1];

void setup() {
  Serial.begin(baud_rate);
  delay(100);
  set_pinModes();
  set_SPI();
  digitalWrite(CE_pin, HIGH);//RX Mode
  digitalWrite(CSN_pin, HIGH);//SPI idle
  SPI.begin();
  set_PipeLength(0, (packet_size+1));//set_PipeLength(pipe 0-5,bytes)
  NRF_get_address(7,1);//NRF_get_address(register,print)

  NRF_bit_write(0, 0, 1);//NRF_bit_write(register,bit,value) RX mode
  NRF_bit_write(0, 1, 1);//register, bit, and value 0,1,1 PowerUP
  NRF_bit_write(0, 4, 1);//RT Mask turns off the RT interrupt
  NRF_bit_write(0, 5, 1);//TX Mask turns off the TX interrupt
  NRF_bit_write(0, 2, 0);//Set CRC
  
  flush_RX_TX();

  NRF_clear_interrupts();
  delay(100);
  attachInterrupt(digitalPinToInterrupt(IRQ_pin), NRF_get_data, FALLING);
  NRF_bit_write(0,6,0);
  NRF_bit_write(1,0,0);
  NRF_bit_write(3,1,0);
  NRF_bit_write(3,0,1);
}

void loop() {
NRF_transmit(10);
}
void NRF_transmit(byte value){
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100001);//flush TX, get rid of anything that might be in there
  digitalWrite(CSN_pin, HIGH);  
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B10100000);//load TX payload
  data_in[0] = SPI.transfer(B00000000);//throw away
  for (int i=1;i<(packet_size+1);i++){
  data_in[i] = SPI.transfer(value);
  }
  digitalWrite(CSN_pin, HIGH);
   
  digitalWrite(CE_pin, LOW);
  delay(1);
  NRF_bit_write(0, 0, 0);//go into TX mode
  delay(1);
  digitalWrite(CE_pin, HIGH);
  delay(1);
  NRF_bit_write(0, 0, 1);//go back into RX mode
  Serial.print(value);
}
void NRF_get_data(){
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B01100001);//read the payload
  for (int i=1; i<(packet_size+1);i++){
  data_in[i] = SPI.transfer(B00000000);
  }
  digitalWrite(CSN_pin, HIGH);
  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100010);//flush RX
  digitalWrite(CSN_pin, HIGH);
  NRF_bit_write(7,6,1);//clear the RX interrupt flag
}
void set_pinModes(){
  pinMode(CE_pin, OUTPUT);//chip enable set as output
  pinMode(CSN_pin, OUTPUT);//chip select pin as output
  pinMode(MOSI_pin, OUTPUT);//SPI data out
  pinMode(MISO_pin, INPUT); //SPI data in
  pinMode(SCK_pin, OUTPUT);//SPI clock out
}
void set_SPI(){
  SPI.setBitOrder(MSBFIRST);//SPI Most Significant Bit First
  SPI.setDataMode(SPI_MODE0);// Mode 0 Rising edge of data, keep clock low
  SPI.setClockDivider(SPI_CLOCK_DIV2);//Run the data in at 16MHz/2 - 8MHz
}
void NRF_get_address(byte address, byte info){  
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(address);
  data_in[1] = SPI.transfer(B00000000);
  digitalWrite(CSN_pin, HIGH);
  if(info==1){//if the user wanted print
    Serial.print("Register Address: ");
    Serial.println(address,BIN);
    Serial.print("Transfer Return: ");
    Serial.println(data_in[0],BIN);
    Serial.print("Address Return: ");
    Serial.println(data_in[1],BIN);
  }
}
void NRF_clear_interrupts(){
  NRF_get_address(7, 0);//RT interrupt
  NRF_bit_write(7,4,1);
  NRF_bit_write(7,5,1);
  NRF_bit_write(7,6,1);
}
void set_PipeLength(byte pipe, byte bytes){
  byte address=pipe+32+16+1;// a register write starts at 32 so add
                            // on the 1 and 16 to get you to at R17
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(address);//write register 11 RX_PW_P0
  data_in[1] = SPI.transfer(bytes);//3 bytes for now
  digitalWrite(CSN_pin, HIGH);
  Serial.print("RX Payload Set Pipe ");
  Serial.print(pipe);
  Serial.print(" for ");
  Serial.print(bytes-1);
  Serial.println(" bytes");
}
void NRF_bit_write(byte address, byte bit_add, byte val){
  NRF_get_address(address, 0);//first read out the register
  if(val==1){
  bitSet(data_in[1],bit_add);
  }
  else{
  bitClear(data_in[1],bit_add);
  }
  digitalWrite(CSN_pin, LOW);//now we'll write the modified data back in
  data_in[0] = SPI.transfer(32+address);//a write to a register adds 32
  data_in[1] = SPI.transfer(data_in[1]);//write the modified register
  digitalWrite(CSN_pin, HIGH);
}
void flush_RX_TX(){
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100010);//flush RX
  digitalWrite(CSN_pin, HIGH);
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100001);//flush TX
  digitalWrite(CSN_pin, HIGH);
}
