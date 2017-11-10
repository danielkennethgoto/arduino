#include<Arduino.h>
class MPU6050
{
  public:
    MPU6050(byte naddress, byte nSDAPin, byte nSCLPin);
    void sendData(byte regaddress, byte data);
    byte getData(byte regaddress);
  private:
    byte address;
    byte SCLPin;
    byte SDAPin;
    byte interruptPin;
    void flipPORTPin(byte pin);
    void flipDDRDPin(byte pin);
    void setDDRDPinHigh(byte pin);
    void setPORTPinHigh(byte pin);
};

