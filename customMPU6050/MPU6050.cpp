#include "MPU6050.h"

MPU6050::MPU6050(byte naddress, byte nSDAPin, byte nSCLPin)
{
  SDAPin = nSDAPin;
  SCLPin = nSCLPin;
  address = naddress << 1; //Gets rid of MSB
  DDRD |= B11111100;  //sets the pins to output, protects pins 0 and 1
  PIND |= SDAPin;
  PIND |= SCLPin;
}
void MPU6050::flipPORTPin(byte pin)
{
  PORTD ^= pin;
}
void MPU6050::flipDDRDPin(byte pin)
{
  DDRD ^= pin;
}
void MPU6050::setDDRDPinHigh(byte pin)
{
  DDRD |= pin;
}
void MPU6050::setPORTPinHigh(byte pin)
{
  PORTD |= pin;
}
void MPU6050::sendData(byte regaddress, byte data)
{
  flipPORTPin(SDAPin);
  byte naddress = address;
  flipPORTPin(SCLPin);
  for (int i = 0; i<7;i++)
  { 
    if ((naddress&B10000000)==B10000000){
      setPORTPinHigh(SDAPin);
      setPORTPinHigh(SCLPin);
      delayMicroseconds(1);
      flipPORTPin(SCLPin);
      flipPORTPin(SDAPin);
    }
    else{
      setPORTPinHigh(B00000000);
      setPORTPinHigh(SCLPin);
      delayMicroseconds(1);
      flipPORTPin(SCLPin);
      flipPORTPin(B00000000);
    }
    naddress <<= B00000001;//get rid of MSB which is a don't care 
    
  } 
  //completed the address, now ask for acknowledgement
  
  for (int i = 0; i<8;i++)
  { 
    if ((regaddress&B10000000)==B10000000){
      setPORTPinHigh(SDAPin);
      setPORTPinHigh(SCLPin);
      delayMicroseconds(1);
      flipPORTPin(SCLPin);
      flipPORTPin(SDAPin);
    }
    else{
      setPORTPinHigh(B00000000);
      setPORTPinHigh(SCLPin);
      delayMicroseconds(1);
      flipPORTPin(SCLPin);
      flipPORTPin(B00000000);
    }
    regaddress <<= B00000001;//get rid of MSB which is a don't care 
    
  }
  setPORTPinHigh(SDAPin);
  setPORTPinHigh(SCLPin);
}
byte MPU6050::getData(byte address)
{
  
}

