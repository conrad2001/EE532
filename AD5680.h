#ifndef AD5680_H
#define AD5680_H
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

/*
 * AD5680 library
 * https://www.analog.com/media/en/technical-documentation/data-sheets/AD5680.pdf
 * developed by: Conrad Chan, Elena Postupalskaya, Tim Jeong
 */

class AD5680{
  private:
    float _vref;
    uint8_t _pin;
    uint8_t _resolution;
    uint32_t _freq;
  public:
    AD5680(uint8_t pin, float vref = 5, uint8_t resolution = 16, uint32_t freq = 1000000);
    void begin();
    void end();
    void set_DAC_Vout(float vout);
};


#endif
