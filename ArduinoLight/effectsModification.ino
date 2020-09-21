//-----------------Модификованые эффекты Гайвера-------
void lighterRoutine() {
  dimAll(125);
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
  if(modes[currentMode].Scale % 2)
  leds[NUM_LEDS - position] = CHSV(modes[currentMode].Scale, 255, 255);
}

void color(){
  fillAll(CHSV(modes[currentMode].Speed, modes[currentMode].Scale, 255));
}

void rainbowRoutine() {
  hue += 2;
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CHSV((byte)(hue + i * modes[currentMode].Scale), 255, 255);
}

//------------------Эффекты от WLED-----------
//----------- Салют 1D------------------------
#define NUM_SPARKS (NUM_LEDS / 2)
float sparkPos[NUM_SPARKS];
float sparkVel[NUM_SPARKS];
float sparkCol[NUM_SPARKS];
float flarePos;

float gravity = -.004; // m/s/s



/*
 * Main Loop
 */
void fireworks() {  
  // send up flare
  flare();
  
  // explode
  explodeLoop();

  // wait before sending up another
  EVERY_N_MILLIS(random16(1000, 4000));
}


/*
 * Send up a flare
 * 
 */
void flare() {
  
  flarePos = 0;
  float flareVel = float(random16(50, 90)) / 100; // trial and error to get reasonable range
  float brightness = 1;

  // initialize launch sparks
  for (int i = 0; i < 5; i++) { sparkPos[i] = 0; sparkVel[i] = (float(random8()) / 255) * (flareVel / 5); // random around 20% of flare velocity sparkCol[i] = sparkVel[i] * 1000; sparkCol[i] = constrain(sparkCol[i], 0, 255); } // launch FastLED.clear(); while (flareVel >= -.2) {
    // sparks
    for (int i = 0; i < 5; i++) {
      sparkPos[i] += sparkVel[i];
      sparkPos[i] = constrain(sparkPos[i], 0, 120);
      sparkVel[i] += gravity;
      sparkCol[i] += -.8;
      sparkCol[i] = constrain(sparkCol[i], 0, 255);
      leds[int(sparkPos[i])] = HeatColor(sparkCol[i]);
      leds[int(sparkPos[i])] %= 50; // reduce brightness to 50/255
    }
    
    // flare
    leds[int(flarePos)] = CHSV(0, 0, int(brightness * 255));
    FastLED.clear();
    flarePos += flareVel;
    flareVel += gravity;
    brightness *= .985;
  }
}

/*
 * Explode!
 * 
 * Explosion happens where the flare ended.
 * Size is proportional to the height.
 */
void explodeLoop() {
  int nSparks = flarePos / 2; // works out to look about right
  
  // initialize sparks
  for (int i = 0; i < nSparks; i++) { 
    sparkPos[i] = flarePos; sparkVel[i] = (float(random16(0, 20000)) / 10000.0) - 1.0; // from -1 to 1 
    sparkCol[i] = abs(sparkVel[i]) * 500; // set colors before scaling velocity to keep them bright 
    sparkCol[i] = constrain(sparkCol[i], 0, 255); 
    sparkVel[i] *= flarePos / NUM_LEDS; // proportional to height 
  } 
  sparkCol[0] = 255; // this will be our known spark 
  float dying_gravity = gravity; 
  float c1 = 120; 
  float c2 = 50; 
  while(sparkCol[0] > c2/128) { // as long as our known spark is lit, work with all the sparks
    FastLED.clear();
    for (int i = 0; i < nSparks; i++) { 
      sparkPos[i] += sparkVel[i]; 
      sparkPos[i] = constrain(sparkPos[i], 0, NUM_LEDS); 
      sparkVel[i] += dying_gravity; 
      sparkCol[i] *= .99; 
      sparkCol[i] = constrain(sparkCol[i], 0, 255); // red cross dissolve 
      if(sparkCol[i] > c1) { // fade white to yellow
        leds[int(sparkPos[i])] = CRGB(255, 255, (255 * (sparkCol[i] - c1)) / (255 - c1));
      }
      else if (sparkCol[i] < c2) { // fade from red to black
        leds[int(sparkPos[i])] = CRGB((255 * sparkCol[i]) / c2, 0, 0);
      }
      else { // fade from yellow to red
        leds[int(sparkPos[i])] = CRGB(255, (255 * (sparkCol[i] - c2)) / (c1 - c2), 0);
      }
    }
    dying_gravity *= .995; // as sparks burn out they fall slower
  }
  FastLED.clear();
}
