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
//----------Мячики------------------
#define GRAVITY           -9.81              // Downward (negative) acceleration of gravity in m/s^2
#define h0                1                  // Starting height, in meters, of the ball (strip length)
#define NUM_BALLS         3                  // Number of bouncing balls you want (recommend < 7, but 20 is fun in its own way)

float h[NUM_BALLS] ;                         // An array of heights
float vImpact0 = sqrt( -2 * GRAVITY * h0 );  // Impact velocity of the ball when it hits the ground if "dropped" from the top of the strip
float vImpact[NUM_BALLS] ;                   // As time goes on the impact velocity will change, so make an array to store those values
float tCycle[NUM_BALLS] ;                    // The time since the last time the ball struck the ground
int   pos[NUM_BALLS] ;                       // The integer position of the dot on the strip (LED index)
long  tLast[NUM_BALLS] ;                     // The clock time of the last ground strike
float COR[NUM_BALLS] ;                       // Coefficient of Restitution (bounce damping)


void BBalls(){
  if (loadingFlag){
  for (int i = 0 ; i < NUM_BALLS ; i++) {    // Initialize variables
    tLast[i] = millis();
    h[i] = h0;
    pos[i] = 0;                              // Balls start on the ground
    vImpact[i] = vImpact0;                   // And "pop" up at vImpact0
    tCycle[i] = 0;
    COR[i] = 0.90 - float(i)/pow(NUM_BALLS,2); 
  }
loadingFlag = false;}

  for (int i = 0 ; i < NUM_BALLS ; i++) {
    tCycle[i] =  millis() - tLast[i] ;     // Calculate the time since the last time the ball was on the ground

    // A little kinematics equation calculates positon as a function of time, acceleration (gravity) and intial velocity
    h[i] = 0.5 * GRAVITY * pow( tCycle[i]/1000 , 2.0 ) + vImpact[i] * tCycle[i]/1000;

    if ( h[i] < 0 ) {                      
      h[i] = 0;                            // If the ball crossed the threshold of the "ground," put it back on the ground
      vImpact[i] = COR[i] * vImpact[i] ;   // and recalculate its new upward velocity as it's old velocity * COR
      tLast[i] = millis();

      if ( vImpact[i] < 0.01 ) vImpact[i] = vImpact0;  // If the ball is barely moving, "pop" it back up at vImpact0
    }
    pos[i] = round( h[i] * (NUM_LEDS - 1) / h0);       // Map "h" to a "pos" integer index position on the LED strip
  }

  //Choose color of LEDs, then the "pos" LED on
  for (int i = 0 ; i < NUM_BALLS ; i++) leds[pos[i]] = CHSV( uint8_t (i * 40) , 255, 255);
  //Then off for the next loop around
  for (int i = 0 ; i < NUM_BALLS ; i++) {
    leds[pos[i]] = CRGB::White;
  }
}
