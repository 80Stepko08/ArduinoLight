//-----------------Модификованые эффекты Гайвера-------
void lighterRoutine() {
  fade();
  if (direction) {
    position++;
    if (position > NUM_LEDS - 2) {
      direction = false;
    }
  } else {
    position--;
    if (position < 1) {
      direction = true;
    }
  }
  leds[position] = CHSV(modes[currentMode].Scale, 255, 255);
}

void color(){
  fillAll(CHSV(modes[currentMode].Speed, modes[currentMode].Scale, 255));
}

void rainbowRoutine() {
  hue += 2;
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CHSV((byte)(hue + i * modes[currentMode].Scale), 255, 255);
}
