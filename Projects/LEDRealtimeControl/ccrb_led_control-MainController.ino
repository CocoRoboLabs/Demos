#include "FastLED.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    3
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    25
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

// data definition
int incomingByte = 0;   // for incoming serial data

void setup() {
  delay(3000);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  Serial1.begin(9600);
}

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop() {

  // send data only when you receive data:
  if (Serial1.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial1.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }
  switch (incomingByte) {
    case 50:
      rainbow();
      FastLED.show();
      FastLED.delay(1000 / FRAMES_PER_SECOND);
      break;
    case 52:
      sinelon();
      FastLED.show();
      FastLED.delay(1000 / FRAMES_PER_SECOND);
      break;
    case 54:
      confetti();
      FastLED.show();
      FastLED.delay(1000 / FRAMES_PER_SECOND);
      break;
    case 58:
      stopAll();
      FastLED.show();
      FastLED.delay(1000 / FRAMES_PER_SECOND);
      break;
  }
  Serial.println(incomingByte, DEC);
  
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13, 0, NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void stopAll()
{
  for ( int i = 0; i < 25; i++ ) {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(10);
  }
}
