

float PSRR_self_test() {
  float supply[2] = {0};
  // ideally ADC9 is also used, but we don't have enough ADCs
  // set switches
  RST_SW();
  set_switches();
  Serial.println(F("Change the external DUT supply voltage to MAX value"));
  supply[0] = set_supply();
  if (!supply[0]) return;  // exit if 'x' is pressed
  ADC7.begin();
  K1 = HIGH, KL2 = HIGH, KL3 = HIGH, K10 = 2 , K22 = HIGH, K23 = HIGH, K24 = HIGH;
  set_switches();
  float LOOP_OUTPUT[2] = {0};
  LOOP_OUTPUT[0] = ADC7.readADC();
  RST_SW();   // reset switches for safety
  set_switches();
  Serial.println(F("Change the external DUT supply voltage to MIN value"));
  supply[1] = set_supply();
  if (!supply[1]) return;  // exit if 'x' is pressed
  K1 = HIGH, KL2 = HIGH, KL3 = HIGH, K10 = 2 , K22 = HIGH, K23 = HIGH, K24 = HIGH;
  set_switches();  // setset switches
  LOOP_OUTPUT[1] = ADC7.readADC();
  ADC7.end();
  return 20 * log10((supply[0] - supply[1] ) / ((LOOP_OUTPUT[0] - LOOP_OUTPUT[1]) / pow(10, K10 + 1)) );
}

float PSRR_2opamp_test() {
  float supply[2] = {0};
  // ideally ADC9 is also used, but we don't have enough ADCs
  // set switches
  RST_SW();
  set_switches();
  Serial.println(F("Change the external DUT supply voltage to MAX value"));
  supply[0] = set_supply();
  if (!supply[0]) return;  // exit if 'x' is pressed
  ADC7.begin();
  K22 = HIGH, K10 = 2, KL2 = HIGH, KL3 = HIGH, K20 = HIGH, K23 = HIGH, K24 = HIGH;
  set_switches();
  float LOOP_OUTPUT[2] = {0};
  LOOP_OUTPUT[0] = ADC7.readADC();
  RST_SW();   // reset switches for safety
  set_switches();
  Serial.println(F("Change the external DUT supply voltage to MIN value"));
  supply[1] = set_supply();
  if (!supply[1]) return;  // exit if 'x' is pressed
  K22 = HIGH, K10 = 2, KL2 = HIGH, KL3 = HIGH, K20 = HIGH, K23 = HIGH, K24 = HIGH;
  set_switches();  // setset switches
  LOOP_OUTPUT[1] = ADC7.readADC();
  ADC7.end();
  return 20 * log10((supply[0] - supply[1] ) / ((LOOP_OUTPUT[0] - LOOP_OUTPUT[1]) / pow(10, K10 + 1)) );
}

float set_supply() {
  Serial.println(F("Enter 's' if ready or 'x' to cancel program"));
  bool start_flg = false, flg = false;
  float voltage = 0;
  typedef enum {get_cmd, get_supply} FSM;
  FSM state = get_cmd;
  while (!start_flg) {
    switch (state) {
      case get_cmd:
        if (Serial.available() > 0) {
          char ch = Serial.read();
          if (ch == 's') {
            Serial.println(F("Enter set DUT supply voltage value: "));
            state = get_supply;
          }
          else if (ch == 'x')   {
            Serial.println(F("PSRR test cancelled"));
            return 0;
          }
        }
        break;
      case get_supply:
        if (Serial.available() > 0) {
          String voltage_str = Serial.readString();
          voltage = voltage_str.toFloat();
          if (flg) {
            if (abs(voltage) > 0 && abs(voltage) < 40) {
              Serial.print(F("DUT voltage entered = "));
              Serial.println(voltage);
              Serial.println(F("PSRR measurement begins, do not change the voltage"));
              start_flg = true;
            }
            else Serial.println(F("Invalid input, please enter value between 1-40"));
          }
          if (!voltage) flg = 1;
        }
        break;
    }
  }
  return voltage;
}
