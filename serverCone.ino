#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "cozyCone";
const char *password = "orenClock";

ESP8266WebServer server(80);

void handleSentVar() {
  if (server.hasArg("awakeTime")) { // this is the variable sent from the client
    int readingInt = server.arg("awakeTime").toInt();
    Serial.println(readingInt);
    //char readingToPrint[5];
    //itoa(readingInt, readingToPrint, 10); //integer to string conversion for OLED library
    //u8g2.firstPage();
    //u8g2.drawUTF8(0, 64, readingToPrint);
    //u8g2.nextPage();
    server.send(200, "text/html", "Data received");
  }
}

void setup() {
  delay(1000);
  Serial.begin(9600);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println(myIP);
  server.on("/data/", HTTP_GET, handleSentVar); // when the server receives a request with /data/ in the string then run the handleSentVar function
  server.begin();
}

void loop() {
  server.handleClient();
}
