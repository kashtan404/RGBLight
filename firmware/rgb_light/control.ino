void incr(int* val, int incr, int limit) {
  *val += incr;
  if (*val > limit) *val = limit;
  if (*val < 0) *val = 0;
}

void LEDon() {
  ONflag = true;
  strip.setPower(1);
}

void LEDoff() {
  ONflag = false;
  strip.setPower(0);
}

void controlTick() {
  enc.tick();

  // ON/OFF
  if (enc.held()) {
    toggleFlag = !toggleFlag;
    if (toggleFlag) LEDoff();
    else LEDon();
  }

  // Brightness
  if (enc.isTurn() && !changeMode && !changeSettings) {
    eeprTimer = millis();
    eeprFlag = true;
    if (enc.right()) {
      if (enc.fast()) {
        thisBright += 5;
      }
      thisBright += 1;
    }
    if (enc.left()) {
      if (enc.fast()) {
        thisBright -= 5;
      }
      thisBright -= 1;
    }
    thisBright = constrain(thisBright, 0, 255);
    strip.setBrightness(thisBright);
  }

  //Modes
  if (enc.isTurn() && changeSettings) {
    if (enc.right())
      incr(&modeNum, 1, MODE_AMOUNT);
    if (enc.left())
      incr(&modeNum, -1, MODE_AMOUNT);
  }

  // Mode settings
  if (enc.isTurn() && changeMode) {
    if (enc.right()) {
      if (enc.fast()) {
          incr(&modeSettings[invSet], 10, maxVals[startFrom[modeNum] + invSet]);
      }
      incr(&modeSettings[invSet], 1, maxVals[startFrom[modeNum] + invSet]);
    }

    if (enc.left()) {
      if (enc.fast()) {
          incr(&modeSettings[invSet], -10, maxVals[startFrom[modeNum] + invSet]);
      }
      incr(&modeSettings[invSet], -1, maxVals[startFrom[modeNum] + invSet]);
    }
  }

  encClicks = enc.hasClicks();
  if (--encClicks <= 2) {
    encMode = encClicks;
  }

  switch (encMode) {
    case 0:   // Switch mode settings
      changeMode = false;
      changeSettings = true;
      break;
    case 1:   // Switch mode
      changeMode = true;
      changeSettings = false;
      break;
    case 2:   // Switch brightness
      changeMode = false;
      changeSettings = false;
      break;
  }
  // Write settings
  if (eeprFlag && millis() - eeprTimer > EEPR_TIME) {
    eeprFlag = false;
    writeSettings();
  }
}
