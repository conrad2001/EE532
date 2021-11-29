

// CS1 switches
uint8_t CS1() {
  // SW8 SW7  SW6 SW5  SW4   SW3 SW2  SW1
  // K17 !K17 KL1 !KL1 K10&2 K10 !K10 K1
  return K1 & 1 | _BV(K10 + 1) | _BV(KL1 + 4) | _BV(K17 + 6) ;
}

uint8_t CS2() {
  // SW8 SW7  SW6 SW5 SW4    SW3     SW2      SW1
  // KL3 !K18 K18 KL2 Aol_in !Aol_in !Aol_out Aol_out
  return KL3 << 7 | _BV(!K18 + 5) | KL2 << 4 | _BV(Aol_in + 2) | _BV(!Aol_out);
}

uint8_t CS3() {
  // SW8  SW7  SW6 SW5 SW4  SW3 SW2  SW1     // SW6 should have been !K24 but we connected to the wrong nodes
  // !K20 K24  -   K23 !K22 K22 !K24 K24
  return !K20 << 7 | K24 << 6 | K23 << 4 | _BV(!K22 + 2) | _BV(!K24);
}


uint8_t CS4() {
  // SW8    SW7    SW6   SW5 SW4 SW3  SW2  SW1
  // DUT25k DUT10k DUT2k K20 K21 !K21 !KL1 KL1
  return DUT25k << 7 | DUT10k << 6 | DUT2k << 5 | K20 << 4 | _BV(K21 + 2) | _BV(!KL1);
}

void RST_SW() {
  // reset switches
  KL1 = LOW, KL2 = LOW, KL3 = LOW, K10 = LOW, K17 = LOW, K18 = LOW, K20 = LOW, K21 = LOW, K22 = LOW, K23 = LOW, K24 = LOW,
  DUT25k = LOW, DUT10k = LOW, DUT2k = LOW,
  Aol_in = LOW, Aol_out = LOW;
}

void set_switches(){
  #define SW_DATA 0x01
#define W 0
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.begin();
  digitalWrite(CS1_PIN, LOW);
  SPI.transfer16( W << 15 | SW_DATA << 14 | CS1());
  digitalWrite(CS1_PIN, HIGH);  // disconnect slave
  digitalWrite(CS2_PIN, LOW);
  SPI.transfer16( W << 15 | SW_DATA << 14 | CS2());
  digitalWrite(CS2_PIN, HIGH);  // disconnect slave
  digitalWrite(CS3_PIN, LOW);
  SPI.transfer16( W << 15 | SW_DATA << 14 | CS3());
  digitalWrite(CS3_PIN, HIGH);  // disconnect slave
  digitalWrite(CS4_PIN, LOW);
  SPI.transfer16( W << 15 | SW_DATA << 14 | CS4());
  digitalWrite(CS4_PIN, HIGH);  // disconnect slave
  SPI.end();
}
