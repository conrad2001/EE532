#ifndef MCP3421_H
#define MCP3421_H
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

/*
 * MCP3421 library 
 * datasheet: http://ww1.microchip.com/downloads/en/devicedoc/22003e.pdf
 * developed by: Conrad Chan, Elena Postupalskaya, Tim Jeong
 */


// conversion modes
#define CONTINUOUS 4
#define RDY 0x80

class MCP3421{
  private:
    uint8_t _addr;
    uint8_t _resolution;    // 3.75 SPS (18 bits)
    uint8_t _gain;
    
  public:
    MCP3421(uint8_t addr, uint8_t resolution = 18, uint8_t gain = 1);
    void begin(void);
    void end(bool stop_ = true);
    float readADC();
    void writeADC(uint8_t _addr, uint8_t _sample_rate, uint8_t _gain_bit);
};

#endif 
