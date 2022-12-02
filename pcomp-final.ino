#include "Wire.h"
#include "Adafruit_TCS34725.h"
#include <FastLED.h>

#define TCAADDR 0x70
#define LED_PIN 2
#define NUM_LEDS 20

CRGB leds[NUM_LEDS];

Adafruit_TCS34725 tcs[] = {Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X),
                           Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X)
                          };
byte gammatable[256];

void setup() {
  Wire.begin();
  Serial.begin(9600);           
                                  
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();

  Serial.println("\nTCAScanner ready!");
  for (uint8_t t = 0; t < 8; t++) {
    tcaselect(t);
    Serial.print("TCA Port #"); Serial.println(t);

    for (uint8_t addr = 0; addr <= 127; addr++) {
      if (addr == TCAADDR) continue;

      Wire.beginTransmission(addr);
      if (!Wire.endTransmission()) {
        Serial.print("Found I2C 0x");  Serial.println(addr, HEX);
      }
    }
  }
  Serial.println("\ndone");

  for (int i = 0; i < 256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
    gammatable[i] = x;
  }  
}

void loop() {
  uint16_t clear0, red0, green0, blue0; 
  uint16_t clear1, red1, green1, blue1; 

  tcaselect(0);
  tcs[0].getRawData(&red0, &green0, &blue0, &clear0);
  Serial.println("--------------sensor 0-------------");
  Serial.print("C:\t"); Serial.print(clear0);                        //print color values
  Serial.print("\tR:\t"); Serial.print(gammatable[(int)red0]);
  Serial.print("\tG:\t"); Serial.print(gammatable[(int)green0]);
  Serial.print("\tB:\t"); Serial.println(gammatable[(int)blue0]);

  tcaselect(1);
  tcs[1].getRawData(&red1, &green1, &blue1, &clear1);
  Serial.println("--------------sensor 1-------------");
  Serial.print("C:\t"); Serial.print(clear1);                        //print color values
  Serial.print("\tR:\t"); Serial.print(gammatable[(int)red1]);
  Serial.print("\tG:\t"); Serial.print(gammatable[(int)green1]);
  Serial.print("\tB:\t"); Serial.println(gammatable[(int)blue1]);

  for (int i = 0; i < NUM_LEDS; i++) {
    if (i % 2 == 0) {
      leds[i] = CRGB(gammatable[(int)red0], gammatable[(int)green0], gammatable[(int)blue0]);
      } else {
        leds[i] = CRGB(gammatable[(int)red1], gammatable[(int)green1], gammatable[(int)blue1]);
        }

    FastLED.setBrightness(6 * i);
    FastLED.show();
  }
  delay(500);
}

void tcaselect(uint8_t i) {
  if (i > 7) return;

  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}
