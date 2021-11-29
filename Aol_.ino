float Aol() {
  RST_SW();
  ADC7.begin();
  ADC13.begin();
  // set switches
  // Loop Control = +5V
  K22 = HIGH, K10 = 1, KL2 = HIGH, KL3 = HIGH, K20 = HIGH, K23 = HIGH, K24 = HIGH, Aol_in = HIGH, Aol_out = HIGH, DUT2k = HIGH;
  set_switches();
  float vout[2] = {0}, LOOP_OUT[2] = {0};
  LOOP_OUT[0] = ADC7.readADC();
  vout[0] = ADC13.readADC();
  Aol_in = LOW, Aol_out = LOW;  // change Loop Control = -5V
  set_switches();
  // read voltages
  LOOP_OUT[1] = ADC7.readADC();
  vout[1] = ADC13.readADC();
  ADC7.end();
  ADC13.end();
  // cal Aol
  return 20 * log10((vout[0] + vout[1]) / ((LOOP_OUT[0] - LOOP_OUT[1]) / pow(10, K10 + 1)));
}
