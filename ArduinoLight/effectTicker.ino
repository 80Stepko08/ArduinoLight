uint32_t effTimer;
byte ind;
void effectsTick() {
  {
    if (ONflag && millis() - effTimer >= ((currentMode < 9  || currentMode > 20) ? modes[currentMode].Speed : 50) ) {
      effTimer = millis(); switch (currentMode) {
          //|номер   |название функции эффекта     |тоже надо|
             case 0 : lighterRoutine();             break;
             case 1 : lightBugs();                  break;
             case 2 : colors();                     break;
             case 3 : color();                      break;
             case 4 : rainbowRoutine();             break;
             case 5 : sparkles();                   break;
             case 6 : fire();                       break;
             case 7 : fireworks();                  break;
             case 8 : BBalls();                     break;

      }
      switch (numHold) {    // индикатор уровня яркости/скорости/масштаба
        case 1:
          ind = sqrt(modes[currentMode].Brightness + 1);
          for (byte y = 0; y < NUM_LEDS ; y++) {
            if (ind > y) leds[y] = CHSV(10, 255, 255);
            else leds[y] =  0;
          }
          break;
        case 2:
          ind = sqrt(modes[currentMode].Speed - 1);
          for (byte y = 0; y <= NUM_LEDS ; y++) {
            if (ind <= y) leds[NUM_LEDS - 1 - y] = CHSV(100, 255, 255);
            else leds[NUM_LEDS - 1 - y] =  0;
          }
          break;
        case 3:
          ind = sqrt(modes[currentMode].Scale + 1);
          for (byte y = 0; y < NUM_LEDS ; y++) {
            if (ind > y) leds[y] = CHSV(150, 255, 255);
            else leds[y] = 0;
          }
          break;

      }
      FastLED.show();
    }
  }
}

void changePower() {    // плавное включение/выключение
  if (ONflag) {
    effectsTick();
    for (int i = 0; i < modes[currentMode].Brightness; i += 8) {
      FastLED.setBrightness(i);
      delay(1);
      FastLED.show();
    }
    FastLED.setBrightness(modes[currentMode].Brightness);
    delay(2);
    FastLED.show();
  } else {
    effectsTick();
    for (int i = modes[currentMode].Brightness; i > 8; i -= 8) {
      FastLED.setBrightness(i);
      delay(1);
      FastLED.show();
    }
    memset8( leds, 0, NUM_LEDS * 3);
    delay(2);
    FastLED.show();
  }
}

void demo() {
  if (isDemo && ONflag && millis() >= DemTimer) {
    if (RANDOM_DEMO)
      currentMode = random8(MODE_AMOUNT); // если нужен следующий случайный эффект
    else
      currentMode = currentMode + 1U < MODE_AMOUNT ? currentMode + 1U : 0U; // если нужен следующий по списку эффект
    memset8( leds, 0, NUM_LEDS * 3);
    DemTimer = millis() + DEMOTIMELIMIT;
    loadingFlag = true;
  }
}
