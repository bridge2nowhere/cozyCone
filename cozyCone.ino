#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_NeoPixel.h>

#define BEDTIMEBUTTON 15
#define PIN 14
#define RELAY_ON 12
#define RELAY_OFF 13


RTC_DS3231 rtc;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(48, PIN, NEO_GRB + NEO_KHZ800);


uint8_t left[] = {39,47,38,46,37,45,36};
uint8_t center[] = {23,31,22,30,21,29,20};
uint8_t right[] = {7,15,6,14,5,13,4};

long bedtimeStart;
uint8_t playHour = 7;
uint8_t playMin = 0;

bool turnOnCar = true;


enum class clockMode : uint8_t
{ 
  Bedtime, Red, Yellow, Green, Day
};

//initializes the time keeper variables
clockMode cMode = clockMode::Red;


void setup() {
  Serial.begin(9600);
  Wire.begin();
  strip.begin();
  strip.show();
  pinMode(12, OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(BEDTIMEBUTTON,INPUT_PULLUP);
  
  //uncomment line below to force time update to the time and date the sketch was compiled
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  bedtime(100);
  bedtime(100);
  bedtime(100);
  bedtime(100);
  
}

void loop() {
  DateTime now = rtc.now();
  Serial.print(now.hour());
  Serial.print(":");
  Serial.println(now.minute());


  if(now.hour() == 19 || (now.hour() == 20 && now.minute() <= 30)) {
    cMode = clockMode::Bedtime;
  }
  else if((now.hour() == 20 && now.minute() > 30) || now.hour() >= 21 || now.hour() < 7) {
    cMode = clockMode::Red;
  }
  else if(now.hour() == 7 && now.minute() >= 0 && now.minute() < 30) {
    cMode = clockMode::Yellow;
  }
  else if((now.hour() == 7 && now.minute() >= 30) || now.hour() == 8) {
    cMode = clockMode::Green;
  }
  else {
    cMode = clockMode::Day;
  }

  if(cMode == clockMode::Bedtime) {
      static int bright = 75;
      if (now.hour() == 20) {
        bright = map(now.minute(), 0, 30, 75, 10);
      }
      bedtime(bright);
  }
  if(cMode == clockMode:: Red) red();
  if(cMode == clockMode :: Yellow) yellow();
  if(cMode == clockMode :: Green) green();
  if(cMode == clockMode :: Day) allOff();
  
}

void bedtime(uint8_t bright) {
    strip.setBrightness(bright);
    strip.setPixelColor(left[4],255,68,0);
    strip.setPixelColor(center[4],255,68,0);
    strip.setPixelColor(right[4],255,68,0);
    strip.setPixelColor(left[5],255,68,0);
    strip.setPixelColor(center[5],255,68,0);
    strip.setPixelColor(right[5],255,68,0);
    strip.show();
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
  delay(1000);
}


void red() {
  digitalWrite(RELAY_OFF,HIGH);
  delay(100);
  digitalWrite(RELAY_OFF,LOW);
  strip.setBrightness(25);
  for (uint8_t i = 0; i < 8; i++) {
    strip.setPixelColor(left[i],255,0,0);
    strip.setPixelColor(right[i],255,0,0);
    strip.setPixelColor(center[i],255,0,0);
  }
  strip.show();
}


void yellow() {
  digitalWrite(RELAY_OFF,HIGH);
  delay(100);
  digitalWrite(RELAY_OFF,LOW);
  strip.setBrightness(50);
  for (uint8_t i = 0; i < 8; i++) {
    strip.setPixelColor(left[i],255,239,0);
    strip.setPixelColor(right[i],255,239,0);
    strip.setPixelColor(center[i],255,239,0);
  }
  strip.show();
}
 


void green() {
  digitalWrite(RELAY_ON,HIGH);
  delay(100);
  digitalWrite(RELAY_ON,LOW);
  strip.setBrightness(100);
  for (uint8_t i = 0; i < 8; i++) {
    strip.setPixelColor(left[i],0,255,0);
    strip.setPixelColor(right[i],0,255,0);
    strip.setPixelColor(center[i],0,255,0);
  }
  strip.show();
}

void allOff() {
  digitalWrite(RELAY_OFF,HIGH);
  delay(100);
  digitalWrite(RELAY_OFF,LOW);
  strip.setBrightness(100);
  for (uint8_t i = 0; i < 8; i++) {
    strip.setPixelColor(left[i],0,0,0);
    strip.setPixelColor(right[i],0,0,0);
    strip.setPixelColor(center[i],0,0,0);
  }
  strip.show();
}
