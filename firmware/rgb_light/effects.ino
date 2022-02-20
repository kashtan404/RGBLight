byte thisBrightness;
int thisColor;
int val, rndVal;
byte fireBright;
uint32_t prevTime;

void effectsTick() {
  if (ONflag) {
    if (modeNum < 2) {
      switch (modeNum) {
        case 0:   // ColorSet
          strip.setHEX(colorVals[modeSettings[0]]);
          thisBrightness = modeSettings[4];
          break;
        case 1:   // Color
          strip.setWheel(modeSettings[1]);
          thisBrightness = modeSettings[4];
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
}

void fireTick() {
  if (millis() - prevTime > modeSettings[2]) {
    rndVal = random(2, 13) * 10;
    prevTime = millis();
  }
  val = val * (1 - SMOOTH_K) + rndVal * SMOOTH_K;
  strip.setWheel(val);
  thisBrightness = map(val, 20, 120, 255, 255);
  strip.setBrightness(thisBrightness);
}

void fireM_tick() {
  if (millis() - prevTime > modeSettings[2]) {
    rndVal = random(0, 13) * 10;
    prevTime = millis();
  }
  val = val * (1 - SMOOTH_K) + rndVal * SMOOTH_K;
  strip.setWheel(modeSettings[3] + val*2);
  thisBrightness = map(val, 0, 120, 255, 255);
  strip.setBrightness(thisBrightness);
}
