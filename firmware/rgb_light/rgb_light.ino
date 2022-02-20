#define SMOOTH_K 0.25

// Driver pin
#define PIN_R 9
#define PIN_G 10
#define PIN_B 3

// Encoder pin
#define CLK 5
#define DT 4
#define SW 6
#define EB_STEP 150
#define EB_CLICK 750

// EEPROM
#define EEPR_TIME 10000
#define EEPR_RESET 1023
#define RESET_VAL 20
#define MODE_ADDR 300

#include <GRGB.h>
GRGB strip(COMMON_CATHODE, PIN_R, PIN_G, PIN_B);

#include <EncButton.h>
EncButton<EB_TICK, CLK, DT, SW> enc(INPUT);

#include <avr/eeprom.h>
#include <EEPROM.h>

#define MODE_AMOUNT 3

int modeSettings[] = {
  0,    // color count for color mode
  0,    // color value for wheel mode
  100,  // blink delay for fire modes
  0,    // color value for manual fire mode
  0,    // brightness
  0,    // previously used mode number
};

const int maxVals[] = {15, 1530, 1000, 1530, 255};

const uint32_t colorVals[] = {
  0xFFFFFF, 0xC0C0C0, 0x808080, 0xFF7F00, 0xFF0000, 0x800000,
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
boolean ONflag = false;
boolean changeMode;
boolean changeSettings;
int thisBright;

void setup() {
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  strip.setCRT(1);
  //randomSeed(analogRead(1));

  // Set PWM
    TCCR1B &= ~_BV(WGM12);
    TCCR1B = TCCR1B & 0b11111000 | 0x01;
    TCCR2A |= _BV(WGM20);
    TCCR2B = TCCR2B & 0b11111000 | 0x01;

  if (eeprom_read_byte((int8_t*)EEPR_RESET) != RESET_VAL) {
    eeprom_write_byte((int8_t*)EEPR_RESET, RESET_VAL);
    writeSettings();
  }

  readSettings();
  modeNum = modeSettings[5];
  thisBright = modeSettings[4];
  delay(100);
}

void loop() {
  controlTick();
  effectsTick();
}
