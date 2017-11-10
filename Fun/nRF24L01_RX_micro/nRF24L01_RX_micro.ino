//NRF      ARDUINO
//1 GND    GND
//2 VCC    3.3V
//3 CE     4  out
//4 CSN    5  out
//5 SCK    13 out
//6 MOSI   11 out
//7 MISO   12 in
//8 IRQ    1  in

// Define your pins here!
#include <SPI.h>


#define CE_pin 4
#define CSN_pin 5
#define IRQ_pin 1
#define MOSI_pin 16
#define MISO_pin 12
#define SCK_pin 13
#define packet_size 4

byte data_in[4];
byte sign;
int x, y;
void setup() {
  start_serial();
  
  delay(100);
  define_pins();
  set_OrderModeSpeed();
  digitalWrite(CE_pin, HIGH);//RX mode
  digitalWrite(CSN_pin, HIGH);//SPI idle
  SPI.begin();//start up the SPI library
  Serial.println("NRF Ready");


  set_PipeLength(0, 5);//pipe 0-5, bytes 1-32
  NRF_get_address(7, 1);//Get Reg7 Status, 1=print screen

  NRFwrite_bit_write(0, 0, 1);//register#, bit#, and value 0 or 1, ::  0,0,1 RX Mode
  NRFwrite_bit_write(0, 1, 1);//register, bit, and value 0,1,1 PowerUP
  NRFwrite_bit_write(0, 4, 1);//RT Mask turns off the RT interrupt
  NRFwrite_bit_write(0, 5, 1);//TX Mask turns off the TX interrupt
  NRFwrite_bit_write(0, 2, 0);//Set CRC to two bytes

  flush_RX_TX();

  NRF_ClearInterrupts();//clears any interrupts
  delay(100);
  attachInterrupt(digitalPinToInterrupt(IRQ_pin), get_data, FALLING);//kick things off by attachin the IRQ interrupt
  NRFwrite_bit_write(0, 6, 0);
  NRFwrite_bit_write(1, 0, 0);
  NRFwrite_bit_write(3, 1, 0);
  NRFwrite_bit_write(3, 0, 1);
}

void loop() {
 
}


void get_data() { // this routine is called when the IRQ pin is pulled LOW by the NRF

  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B01100001);//read the payload
  data_in[0] = SPI.transfer(B00000000);
  data_in[1] = SPI.transfer(B00000000);
  data_in[2] = SPI.transfer(B00000000);
  data_in[3] = SPI.transfer(B00000000);
  data_in[4] = SPI.transfer(B00000000);
  digitalWrite(CSN_pin, HIGH);

  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100010);//flush RX
  digitalWrite(CSN_pin, HIGH);
  NRFwrite_bit_write(7, 6, 1); //clear the RX interrupt flag
  Serial.print(data_in[2]);
  Serial.print("     ");
  Serial.print(data_in[3]);
  Serial.print("     ");
  Serial.println(data_in[4]);
  if (bitRead(data_in[4],1)){
    y = 0-data_in[2];
  }
  else{
    y = data_in[2];
  }
  if (bitRead(data_in[4],2)){
    x = data_in[3];
  }
  else{
    x = 0-data_in[3];
  }
  Serial.println(1);
  if (bitRead(data_in[4],3)){
    Serial.println(1);
  }
  else{
    Serial.println(1);
  }
}
void NRF_ClearInterrupts() { //    start clear interrupts      start clear interrupts      start clear interrupts
  //there are three interrupt flags in the NRF.  Thsi routine checks them, and if set, it will clear them

  NRF_get_address(7, 0);//RT interrupt
  if (bitRead(data_in[1], 4))
    NRFwrite_bit_write(7, 4, 1);

  NRF_get_address(7, 0);//TX interrupt
  if (bitRead(data_in[1], 5))
    NRFwrite_bit_write(7, 5, 1);

  NRF_get_address(7, 0);//RX interrupt
  if (bitRead(data_in[1], 6))
    NRFwrite_bit_write(7, 6, 1);

}
void start_serial() {
  Serial.begin(115200);
  Serial.println("Serial begin...");
}
void define_pins() {
  pinMode(CE_pin, OUTPUT);//chip enable set as output
  pinMode(CSN_pin, OUTPUT);//chip select pin as output
  pinMode(MOSI_pin, OUTPUT);//SPI data out
  pinMode(MISO_pin, INPUT); //SPI data in
  pinMode(SCK_pin, OUTPUT);//SPI clock out
  Serial.println("NRF Pins Initialized");
}
void set_OrderModeSpeed() {
  SPI.setBitOrder(MSBFIRST);//SPI Most Significant Bit First
  SPI.setDataMode(SPI_MODE0);// Mode 0 Rising edge of data, keep clock low
  SPI.setClockDivider(SPI_CLOCK_DIV2);//Run the data in at 16MHz/2 - 8MHz
}
void set_PipeLength(byte pipe, byte bytes) { //start NRF payload     start NRF payload
  byte address = pipe + 32 + 16 + 1; // a register write starts at 32, so add on the 1 and 16 to get you to at R17
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

void flush_RX_TX() {
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100010);//flush RX
  digitalWrite(CSN_pin, HIGH);
  digitalWrite(CSN_pin, LOW);
  data_in[0] = SPI.transfer(B11100001);//flush TX
  digitalWrite(CSN_pin, HIGH);
}
void NRFwrite_bit_write(byte address, byte bit_add, byte val) { //   start bit write   start bit write   start bit write
  //This routine writes single bits of a register, without affecting the rest of the register

  NRF_get_address(address, 0);//first read out the register
  if (val == 1) //if we want to write a one to the bit then set the bit in the register we read
    bitSet(data_in[1], bit_add);
  else
    bitClear(data_in[1], bit_add); //clear it if not

  digitalWrite(CSN_pin, LOW);//now we'll write the modified data back in
  data_in[0] = SPI.transfer(32 + address); //a write to a register adds 32
  data_in[1] = SPI.transfer(data_in[1]);//write the modified register
  digitalWrite(CSN_pin, HIGH);
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


