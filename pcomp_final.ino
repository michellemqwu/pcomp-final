#include "Adafruit_TCS34725.h"
#include <FastLED.h>
#define LED_PIN 2
#define NUM_LEDS 20

CRGB leds[NUM_LEDS];

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

byte gammatable[256];

void setup() {
  Serial.begin(9600);                                             //Sart serial comms @ 9600 (you can change this)
  Serial.println("Color View Test");                              //Title info

  if (tcs.begin()) {                                              //if the sensor starts correctly
    Serial.println("Found sensor");                               //print the happy message
  } else {                                                        //if the sensor starts incorrectly
    Serial.println("No TCS34725 found ... check your connections");//print the not so happy message
    while (1); // halt!
  }

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();

  for (int i = 0; i < 256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;
    gammatable[i] = x;

    //Serial.println(gammatable[i]);
  }  
}

void loop() {
  uint16_t clear, red, green, blue; 
  
  tcs.setInterrupt(false);                                          // turn on LED

  //delay(60);                                                        // takes 50ms to read

  tcs.getRawData(&red, &green, &blue, &clear);                      //read the sensor

  tcs.setInterrupt(true);                                           // turn off LED

  Serial.print("C:\t"); Serial.print(clear);                        //print color values
  Serial.print("\tR:\t"); Serial.print(gammatable[(int)red]);
  Serial.print("\tG:\t"); Serial.print(gammatable[(int)green]);
  Serial.print("\tB:\t"); Serial.println(gammatable[(int)blue]);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(gammatable[(int)red], gammatable[(int)green], gammatable[(int)blue]);
    FastLED.setBrightness(6 * i);
    FastLED.show();
    delay(50);
  }
}
