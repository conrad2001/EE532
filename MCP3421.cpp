/*
   MCP3421 library
   datasheet: http://ww1.microchip.com/downloads/en/devicedoc/22003e.pdf
   developed by: Conrad Chan, Elena Postupalskaya, Tim Jeong
*/

#include "MCP3421.h"
#include <Wire.h>

MCP3421::MCP3421(uint8_t addr, uint8_t resolution, uint8_t gain) {
  _addr = addr;
  _resolution = resolution;
  _gain = gain;
}

void MCP3421::begin(void) {
  Wire.begin();
  uint8_t _gain_bit, _sample_rate;
  switch (_resolution) {
    case 12: _sample_rate = 0; // 240 SPS (12 bits)
    case 14: _sample_rate = 1; // 60 SPS (14 bits)
    case 16: _sample_rate = 2; // 15 SPS (16 bits)
    case 18: _sample_rate = 3; // 3.75 SPS (18 bits)
    default: _sample_rate = 3;
  }
  switch (_gain) {
    case 1: _gain_bit = 0;
    case 2: _gain_bit = 1;
    case 4: _gain_bit = 2;
    case 8: _gain_bit = 3;
    default: _gain_bit = 0;
  }
  writeADC(_addr, _sample_rate, _gain_bit);
}

void MCP3421::end(bool stop_) {
  Wire.endTransmission(stop_);
}

float MCP3421::readADC() {
  uint32_t output_code = 0;
  Wire.requestFrom((uint8_t)_addr, (uint8_t)3, (uint8_t)false);
  while (Wire.available())
    output_code = output_code << 8 | Wire.read();
  output_code &= 0x3FFFF; // mask garbage bits 18-24
  /*If MSB = 0 (Positive Output Code):  Input voltage = output code * LSB / PGA
    If MSB = 1 (Negative Output Code):  Input voltage = 2’s complement of output code * LSB / PGA
    Where:
    LSB = 4.096/2^resolution
    2’s complement = 1’s complement + 1
  */
  return (0x20000 & output_code ? ~output_code + 1 : output_code) * 4.096 / (float)((uint32_t) 1 << _resolution) / _gain;
}

void MCP3421::writeADC(uint8_t _addr, uint8_t _sample_rate, uint8_t _gain_bit) {
  Wire.beginTransmission(_addr);
  Wire.write(_BV(CONTINUOUS) | (_sample_rate & 0x03) << 2 | _gain_bit & 0x03 );
  Wire.endTransmission();
}
