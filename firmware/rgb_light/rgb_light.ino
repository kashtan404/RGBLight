#define SMOOTH_K 0.25

// Driver pin
#define PIN_R 9
#define PIN_G 10
#define PIN_B 3

// Encoder pin
#define CLK 4
#define DT 5
#define SW 6
#define EB_STEP 150
#define EB_CLICK 700

// EEPROM
#define EEPR_TIME 10000
#define EEPR_RESET 1023
#define RESET_VAL 20
#define MODE_ADDR 300
#define MODE_START 20

#include <GRGB.h>
GRGB strip(COMMON_CATHODE, PIN_R, PIN_G, PIN_B);

#include <EncButton.h>
EncButton<EB_TICK, CLK, DT, SW> enc(INPUT);

#include <avr/eeprom.h>
#define EEPROM_UPD_BYTE(addr, value) eeprom_update_byte((uint8_t*)(addr), (value))
#define EEPROM_UPD_WORD(addr, value) eeprom_update_word((uint16_t*)(addr), (uint16_t)(value))

#define MODE_AMOUNT 4

int modeSettings[] = {255, 15, 255, 255, 1000};
const byte setAmount[] = {5, 4, 3, 3, 3, 4, 4, 5, 5, 5, 3, 5};
const byte startFrom[] = {0, 4, 7, 9, 11, 13, 16, 19, 23, 27, 31, 33};

const int maxVals[] = {255, 15, 255, 255, 1000};

const uint32_t colorVals[] = {
  0xFFFFFF, 0xC0C0C0, 0x808080, 0x000000, 0xFF0000, 0x800000,
  0xFFFF00, 0x808000, 0x00FF00, 0x008000, 0x00FFFF, 0x008080,
  0x0000FF, 0x000080, 0xFF00FF, 0x800080,
};

uint32_t eeprTimer;
uint32_t effTimer;
uint16_t effPeriod;
boolean eeprFlag;
int16_t modeNum = 0;
int8_t encMode = 2;
int8_t encClicks = 3;
int invSet = 0;
boolean ONflag = true;
boolean toggleFlag;
boolean changeMode;
boolean changeSettings;
int thisBright = 255;

void setup() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);

  randomSeed(analogRead(1));

  // Set PWM
    TCCR1B &= ~_BV(WGM12);
    TCCR1B = TCCR1B & 0b11111000 | 0x01;
    TCCR2A |= _BV(WGM20);
    TCCR2B = TCCR2B & 0b11111000 | 0x01;

  // Check first execution
  if (eeprom_read_byte((uint8_t*)EEPR_RESET) != RESET_VAL) {
    eeprom_write_byte((uint8_t*)EEPR_RESET, RESET_VAL);
    writeSettings();
  }
  readSettings();
  delay(100);
}

void loop() {
  controlTick();
  effectsTick();
}
