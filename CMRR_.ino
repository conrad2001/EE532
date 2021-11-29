float CMRR_self_test() {
  RST_SW() ;
  ADC7.begin();
  ADC8.begin();
  // set switches
  K1 = HIGH, KL2 = HIGH, KL3 = HIGH, K10 = 2 , K18 = HIGH, K22 = HIGH, K23 = HIGH, K24 = HIGH;
  set_switches();
  // set DAC_V = 2mV
  DAC6.begin();
  DAC6.set_DAC_Vout(0.002);
  DAC6.end();
  float LOOP_OUTPUT[2] = {0}, CONTI_INP[2] = {0};
  LOOP_OUTPUT[0] = ADC7.readADC();
  CONTI_INP[0] = ADC8.readADC();
  //set DAC_V = 6mV
  DAC6.begin();
  DAC6.set_DAC_Vout(0.006);
  DAC6.end();
  LOOP_OUTPUT[1] = ADC7.readADC();
  CONTI_INP[1] = ADC8.readADC();
  ADC7.end(true);
  ADC8.end(true);
  return 20 * log10((LOOP_OUTPUT[1] - LOOP_OUTPUT[0]) / (CONTI_INP[1] - CONTI_INP[0]));
}


float CMRR_2opamp_test() {
  RST_SW() ;
  ADC7.begin();
  ADC8.begin();
  // set switches
  K22 = HIGH, K10 = 2, KL2 = HIGH, KL3 = HIGH, K18 = HIGH, K20 = HIGH, K23 = HIGH, K24 = HIGH;
  set_switches();
  // set DAC_V = 2mV
  DAC6.begin();
  DAC6.set_DAC_Vout(0.002);
  DAC6.end();
  float LOOP_OUTPUT[2] = {0}, CONTI_INP[2] = {0};
  LOOP_OUTPUT[0] = ADC7.readADC();
  CONTI_INP[0] = ADC8.readADC();
  // set DAC_V = 6mV
  DAC6.begin();
  DAC6.set_DAC_Vout(0.006);
  DAC6.end();
  LOOP_OUTPUT[1] = ADC7.readADC();
  CONTI_INP[1] = ADC8.readADC();
  ADC7.end(true);
  ADC8.end(true);
  return 20 * log10((LOOP_OUTPUT[1] - LOOP_OUTPUT[0]) / (CONTI_INP[1] - CONTI_INP[0]));
}
