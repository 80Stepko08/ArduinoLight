/*
  Скетч к проекту "Огненный светильник"
  Страница проекта (схемы, описания): https://alexgyver.ru/gyverlight/
  Нравится, как написан код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver, AlexGyver Technologies, 2019 
 основа от Norovl(gyverlamp for arduino),доработка от stepko365
  https://AlexGyver.ru/
*/
// ---------------- БИБЛИОТЕКИ -----------------
#include <EEPROM.h>
#include <FastLED.h>
#include <GyverButtonOld.h>
//-----------------            -----------------
#include "Constants.h"
// ----------------- ПЕРЕМЕННЫЕ ------------------
struct {
  byte Brightness = 255;
  byte Speed = 125;
  byte Scale = 101;
} modes[MODE_AMOUNT]; //настройки эффекта по умолчанию
int8_t currentMode = 1;
boolean loadingFlag = true;
boolean ONflag = true;
byte numHold;
unsigned long numHold_Timer = 0;


void setup() {
  // ЛЕНТА
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)/*.setCorrection( TypicalLEDStrip )*/;
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT/NUM_STRIPS);
  memset8( leds, 0, NUM_LEDS * 3);
  FastLED.show();

  touch.setStepTimeout(100);
  touch.setClickTimeout(500);

  if (EEPROM.read(0) == 102) {                    // если было сохранение настроек, то восстанавливаем их (с)НР
    currentMode = EEPROM.read(1);
    for (byte x = 0; x < MODE_AMOUNT; x++) {
      modes[x].Brightness = EEPROM.read(x * 3 + 11); // (2-10 байт - резерв)
      modes[x].Speed = EEPROM.read(x * 3 + 12);
      modes[x].Scale = EEPROM.read(x * 3 + 13);
    }
  }
}

void loop() {
  effectsTick();
  buttonTick();
  demo();
}
