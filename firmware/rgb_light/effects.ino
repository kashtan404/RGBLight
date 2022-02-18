byte thisBrightness;
int thisColor;
boolean strobeFlag;
float k = 0.25;
int val, rndVal;
byte fireBright;
uint32_t prevTime;
byte policeState = 0;
bool flashState = false;

void effectsTick() {
  if (modeNum < 2) {
    switch (modeNum) {
      case 0:   // Color
        strip.setWheel8(modeSettings[2]);
        thisBrightness = modeSettings[0];
        break;
      case 1:   // ColorSet
        strip.setHEX(colorVals[modeSettings[1]]);
        thisBrightness = modeSettings[0];
        break;
    }
    strip.setBrightness(thisBrightness);
  } else {
    if (millis() - effTimer >= effPeriod) {
      effTimer = millis();
      switch (modeNum) {
        case 2:   // Fire
          effPeriod = 40;
          fireTick();
          break;
        case 3:   // FireManual
          effPeriod = 40;
          fireM_tick();
          break;
      }
    }
  }
}

void fireTick() {
  if (millis() - prevTime > modeSettings[4]) {
    rndVal = random(2, 13) * 10;
    prevTime = millis();
  }
  val = val * (1 - SMOOTH_K) + rndVal * SMOOTH_K;
  strip.setWheel8(val);
  thisBrightness = map(val, 20, 120, 255, 255);
  strip.setBrightness(thisBrightness);
}

void fireM_tick() {
  if (millis() - prevTime > modeSettings[4]) {
    rndVal = random(0, 13) * 10;
    prevTime = millis();
  }
  val = val * (1 - SMOOTH_K) + rndVal * SMOOTH_K;
  thisBrightness = map(val, 0, 120, 255, 255);
  strip.setWheel8(modeSettings[3] * 6);
  strip.setBrightness(thisBrightness);
}
