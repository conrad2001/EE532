float VOS_self_test() {
  RST_SW();
  ADC7.begin();
  // set switches
  K1 = HIGH, KL2 = HIGH, KL3 = HIGH, K10 = 2 , K22 = HIGH, K23 = HIGH, K24 = HIGH;
  set_switches();
  float ADC_val = ADC7.readADC();
  ADC7.end(true);
  return ADC_val / pow(10, K10 + 1);
}

float VOS_2opamp_test() {
  RST_SW() ;
  ADC7.begin();
  // set switches
  K22 = HIGH, K10 = 2, KL2 = HIGH, KL3 = HIGH, K20 = HIGH, K23 = HIGH, K24 = HIGH;
  set_switches();
  float ADC_val = ADC7.readADC();
  ADC7.end(true);
  return ADC_val / pow(10, K10 + 1);
}
