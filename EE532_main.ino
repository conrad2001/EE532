/* note
   write DAC code
   short DUT_LOAD to GND during AOL test
*/
#include <math.h>
#include <Wire.h>
#include <SPI.h>
#include "MCP3421.h"
#include "AD5680.h"

#define CS1_PIN 6
#define CS2_PIN 9
#define CS3_PIN 8
#define CS4_PIN 7
#define CS5_PIN 10
#define RST 5

#define MCP3421A0 0x68
#define MCP3421A1 0x69
#define MCP3421A2 0x6A
#define MCP3421A3 0x6B

MCP3421 ADC7(MCP3421A1);// create instance of class MCP3421
MCP3421 ADC8(MCP3421A2);
MCP3421 ADC13(MCP3421A3);
AD5680 DAC6(CS5_PIN);

uint8_t K1 = LOW, KL1 = LOW, KL2 = LOW, KL3 = LOW, K10 = LOW, K17 = LOW, K18 = LOW, K20 = LOW, K21 = LOW, K22 = LOW, K23 = LOW, K24 = LOW,
        DUT25k = LOW, DUT10k = LOW, DUT2k = LOW,
        Aol_in = LOW, Aol_out = LOW;

typedef enum {chill, ALL_ST, VOS_ST, PSRR_ST, CMRR_ST, AOL_ST, print_data} FSM;
FSM state = chill;
bool ready_flg = 0, all_test = 0, clr = 0;
float result[4][10] = {0};
uint16_t test_num = -1;
float val = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(CS1_PIN, OUTPUT);
  pinMode(CS2_PIN, OUTPUT);
  pinMode(CS3_PIN, OUTPUT);
  pinMode(CS4_PIN, OUTPUT);
  digitalWrite(CS1_PIN, HIGH);
  digitalWrite(CS2_PIN, HIGH);
  digitalWrite(CS3_PIN, HIGH);
  digitalWrite(CS4_PIN, HIGH);
  pinMode(RST, OUTPUT);
  digitalWrite(RST, HIGH);

  Serial.begin(9600);
  delay(1);
  DUT25k = 1, DUT10k = 0, DUT2k = 1, K20 = HIGH, K21 = LOW, K24 = HIGH, K23 = LOW, K22 = HIGH, K17 = 1, KL1 = 1, K10 = 2, K1 = 1;
  Serial.println("begin");  
}

void loop() {
  // put your main code here, to run repeatedly:
  user_interface();
}

void user_interface(){
    switch (state) {
    case chill:
      Serial.println("\nPlease select test from the following:");
      Serial.println(F("All tests -> enter 1"));
      Serial.println(F("Offset Voltage -> enter 2"));
      Serial.println(F("PSRR -> enter 3"));
      Serial.println(F("CMRR -> enter 4"));
      Serial.println(F("Open Loop Gain -> enter 5"));
      Serial.println(F("Check 10 recent results -> enter 6"));
      Serial.read();
      ready_flg = 0;
      all_test = 0;
      while (!ready_flg) {
        if (Serial.available() > 0) {
          uint8_t num = Serial.read() - 48;
          if (num > 0 && num <= 6) {
            ready_flg = 1;
            state = num;
            if (state != print_data) {
              test_num++;
              if (test_num > 10) clr = 1;
              // clear old datas
              if (clr)
                for (int i = 0 ; i < 4 ; i++)
                  result[i][test_num % 10] = 0;
            }
          }
          else {
            Serial.println(F("Invalid input, please enter again"));
            while (Serial.read());
          }
        }
      }
      break;
    case ALL_ST:
      all_test = 1;
      state = VOS_ST;
      break;
    case VOS_ST:
      Serial.println(F("\nOffset Voltage Test:"));
      Serial.println(F("Select test type from the following"));
      Serial.println(F("Self Test -> enter 1"));
      Serial.println(F("2-opamp test -> enter 2"));
      Serial.read();
      ready_flg = 0;
      while (!ready_flg) {
        if (Serial.available() > 0) {
          uint8_t num = Serial.read() - 48;
          if (num > 0 && num < 3) {
            result[0][test_num % 10] = num == 1 ? VOS_self_test() : VOS_2opamp_test();
            Serial.print(F("\nThe Offset Voltage of the DUT is "));
            Serial.print(result[0][test_num % 10]);
            Serial.println("V");
            ready_flg = 1;
            state = all_test ? PSRR_ST : chill;
          }
          else Serial.println(F("Invalid input, please enter again"));
        }
      }
      break;
    case PSRR_ST:
      Serial.println(F("\nPSRR Test:"));
      Serial.println(F("Select test type from the following"));
      Serial.println(F("Self Test -> enter 1"));
      Serial.println(F("2-opamp test -> enter 2"));
      Serial.read();
      ready_flg = 0;
      while (!ready_flg) {
        if (Serial.available() > 0) {
          uint8_t num = Serial.read() - 48;
          if (num > 0 && num < 3) {
            result[1][test_num % 10] = num == 1 ? PSRR_self_test() : PSRR_2opamp_test();
            Serial.print(F("\nThe PSRR of the DUT is "));
            Serial.print(result[1][test_num % 10]);
            Serial.println("dB");
            ready_flg = 1;
            state = all_test ? CMRR_ST : chill;
          }
          else Serial.println(F("Invalid input, please enter again"));
        }
      }
      break;
    case CMRR_ST:
      Serial.println(F("\nCMRR Test:"));
      Serial.println(F("Select test type from the following"));
      Serial.println(F("Self Test -> enter 1"));
      Serial.println(F("2-opamp test -> enter 2"));
      Serial.read();
      ready_flg = 0;
      while (!ready_flg) {
        if (Serial.available() > 0) {
          uint8_t num = Serial.read() - 48;
          if (num > 0 && num < 3) {
            result[2][test_num % 10] = num == 1 ? CMRR_self_test() : CMRR_2opamp_test();
            Serial.print(F("\nThe CMRR of the DUT is "));
            Serial.print(result[2][test_num % 10]);
            Serial.println("dB");
            ready_flg = 1;
            state = all_test ? AOL_ST : chill;
          }
          else Serial.println(F("Invalid input, please enter again"));
        }
      }
      break;
    case AOL_ST:
      Serial.println(F("\nOpen Loop Gain Test:"));
      result[3][test_num % 10] = Aol();
      Serial.print(F("\nThe Open Loop Gain of the DUT is "));
      Serial.print(result[3][test_num % 10]);
      Serial.println("dB");
      state = chill;
      break;
    case print_data:
      Serial.println(F("\nTEST   VOS[V]    PSRR[dB]  CMRR[dB]  AOL[dB]"));
      uint16_t temp = test_num + 1;
      uint8_t counter = 0;
      while (temp > 0 && counter < 10) {
        Serial.print(F("RUN "));
        Serial.print(temp);
        for (int i = 0; i < 4; i++)  {
          Serial.print(F("  "));
          if (!result[i][(temp - 1) % 10]) Serial.print(F("    "));
          else Serial.print(result[i][(temp - 1) % 10]);
          Serial.print(F("    "));
        }
        Serial.println("");
        temp--;
        counter++;
      }
      state = chill;
      break;
    default: state = chill;
  }
}
