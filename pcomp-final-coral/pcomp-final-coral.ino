#include "Wire.h"
#include "Adafruit_TCS34725.h"
#include <FastLED.h>

#define TCAADDR 0x70
#define LED_PIN 2
#define NUM_LEDS 144

CRGB leds[NUM_LEDS];

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
Adafruit_TCS34725 tcs1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

byte gammatable[256];

void setup() {
  Wire.begin();
  Serial.begin(9600);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  for (int i = 0; i < 256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
    gammatable[i] = x;
  }
}

void loop() {
  tcaselect(0);
  float red, green, blue;
  tcs.getRGB(&red, &green, &blue);
  Serial.println("--------------sensor 0-------------");
  Serial.print("\tR:\t"); Serial.print(gammatable[(int)red]);
  Serial.print("\tG:\t"); Serial.print(gammatable[(int)green]);
  Serial.print("\tB:\t"); Serial.println(gammatable[(int)blue]);

  tcaselect(1);
  float red1, green1, blue1;
  tcs1.getRGB(&red1, &green1, &blue1);
  Serial.println("--------------sensor 1-------------");
  Serial.print("\tR:\t"); Serial.print(gammatable[(int)red1]);
  Serial.print("\tG:\t"); Serial.print(gammatable[(int)green1]);
  Serial.print("\tB:\t"); Serial.println(gammatable[(int)blue1]);

  for (int i = NUM_LEDS / 2; i < NUM_LEDS; i++) {
    if (i % 2 == 0) {
      leds[i] = CRGB(gammatable[(int)red], gammatable[(int)green], gammatable[(int)blue]);
    } else {
      leds[i] = CRGB(gammatable[(int)red1], gammatable[(int)green1], gammatable[(int)blue1]);
    }
    FastLED.show();
    delay(10);
  }

  for (int i = NUM_LEDS / 2; i > -1; i--) {
    if (i % 2 == 0) {
      leds[i] = CRGB(gammatable[(int)red], gammatable[(int)green], gammatable[(int)blue]);
    } else {
      leds[i] = CRGB(gammatable[(int)red1], gammatable[(int)green1], gammatable[(int)blue1]);
    }
    FastLED.show();
    delay(10);
  }
}

void tcaselect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}
