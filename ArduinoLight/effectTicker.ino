uint32_t effTimer;
byte ind;
void effectsTick() {
  {
    if (ONflag && millis() - effTimer >= ((currentMode < 10  || currentMode > 20) ? modes[currentMode].Speed : 50) ) {
      effTimer = millis(); switch (currentMode) {
          //|номер   |название функции эффекта     |тоже надо|
             case 0 : lighterRoutine();             break;
             case 1 : lightBugs();                  break;
             case 2 : colors();                     break;
             case 3 : color();                      break;
             case 4 : rainbowRoutine();             break;
             case 5 : sparkles();                   break;
             case 6 : fire();                       break;
             case 7 : NoiseRoutine();               break;

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
