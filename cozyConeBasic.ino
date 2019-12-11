#include <Adafruit_NeoPixel.h>

#define PIN 14


Adafruit_NeoPixel strip = Adafruit_NeoPixel(48, PIN, NEO_GRB + NEO_KHZ800);

  uint8_t left[] = {39,47,38,46,37,45,36};
  uint8_t center[] = {23,31,22,30,21,29,20};
  uint8_t right[] = {7,15,6,14,5,13,4}; 


void setup() {


  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(100);
  strip.setPixelColor(left[4],255,68,0);
  strip.setPixelColor(center[4],255,68,0);
  strip.setPixelColor(right[4],255,68,0);
  strip.setPixelColor(left[5],255,68,0);
  strip.setPixelColor(center[5],255,68,0);
  strip.setPixelColor(right[5],255,68,0);
  strip.show();
}

void loop() {
 
  for(int i = 3; i >= 0 ; i--) {
    strip.setPixelColor(left[i],200,200,255);
    strip.setPixelColor(right[i],200,200,255);
    strip.setPixelColor(center[i],200,200,255);
    strip.show();
    delay(1000);
    strip.setPixelColor(left[i],0,0,0);
    strip.setPixelColor(right[i],0,0,0);
    strip.setPixelColor(center[i],0,0,0);
    strip.show();
  }
}
