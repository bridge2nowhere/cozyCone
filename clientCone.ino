#include <ESP8266WiFi.h>
#include <RotaryEncoder.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

SSD1306AsciiWire oled;
RotaryEncoder encoder(13, 12);

const char *ssid = "cozyCone";
const char *password = "orenClock";

int outputValue = 890000;        // value sent to server

void setup() {
  Serial.begin(9600);
  delay(10);
  Wire.begin();
  Wire.setClock(400000L);  
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(Verdana12);
  oled.clear();
  // Explicitly set the ESP8266 to be a WiFi-client
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    oled.println("connecting");
  }
   pinMode(14, INPUT_PULLUP);
   oled.clear();
   oled.println("ready");
}

void loop() {
  static int pos = 0;
  static uint8_t awakeHour = 0;
  static uint8_t awakeMin = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {
    if (newPos < pos) awakeHour++;
    if (newPos > pos) awakeMin++;
    if (awakeHour >= 24) awakeHour = 0;
    if (awakeMin >=60) awakeMin = 0;
    oled.clear();
    oled.println();
    oled.println();
    oled.print(awakeHour);
    oled.print(":");
    if (awakeMin < 10) oled.print("0");
    oled.println(awakeMin);
    pos = newPos;

  }



  /////////////////////////////////////////////
  //Serial.println(outputValue);
  if (digitalRead(14) == LOW) {
    Serial.println("click");
    uploadTime((awakeHour * 60) + awakeMin);
  }
  

  
}

void uploadTime(long timeSend) {
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const char * host = "192.168.4.1";
  const int httpPort = 80;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request. Something like /data/?sensor_reading=123
  String url = "/data/";
  url += "?awakeTime=";
  url += timeSend;

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  delay(500);
}
