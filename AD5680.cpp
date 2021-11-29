#include "AD5680.h"
#include <SPI.h>

AD5680::AD5680(uint8_t pin, float vref, uint8_t resolution, uint32_t freq) {
  _vref = vref;
  _resolution = resolution;
  _pin = pin;
  _freq = freq;
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, HIGH);
}

void AD5680::begin(){ 
  SPI.beginTransaction(SPISettings(_freq, MSBFIRST, SPI_MODE1));
  SPI.begin();
}

void AD5680::end(){
  SPI.end();
}

void AD5680::set_DAC_Vout(float vout) {
  uint32_t D = vout * 262144 / _vref;
  if (_resolution == 18) D &= 0x7FFFF;
  else D &= 0x7FFFC;
  D = D << 2;
  digitalWrite(_pin, LOW);
  SPI.transfer(&D, sizeof(D));
  digitalWrite(_pin, HIGH);
}
