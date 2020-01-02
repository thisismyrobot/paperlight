/*
 * Add https://dl.espressif.com/dl/package_esp32_index.json as an additional board manager URL.
 * Install "esp32" board via the board manager.
 * Install the "M5StickC 0.1.1" library via Manage Libraries.
 * Select "M5Stick-C" in the boards manager.
 * Select 115200 as the upload speed.
 */
#include <M5StickC.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "settings.h"

const char* host = "papertrailapp.com";
const int port = 443;
String path = String("/api/v1/events/search.json") + papertrailquery;

void setup() {
  M5.begin();
  M5.Axp.ScreenBreath(8);

  M5.Lcd.print("Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      M5.Lcd.print(".");
  }
  M5.Lcd.print("\n\nConnected!\n");

  M5.Lcd.print("\nLoading");
  WiFiClientSecure client;
  client.connect(host, port);
  client.println(String("GET ") + path + " HTTP/1.1");
  client.println(String("Host: ") + host);
  client.println(String("X-Papertrail-Token: ") + papertrailtoken);
  client.println();

  bool found = false;
  while (client.connected()) {
    M5.Lcd.print(".");
    if(client.readStringUntil('\n').indexOf("\"id\"") > 0) {
      found = true;
      break;
    }
  }
  client.stop();
  M5.Lcd.print("\n\nLoaded!\n");

  M5.Lcd.print("\n");
  if (found) {
    M5.Lcd.print("Found!");     
  }
  else {
    M5.Lcd.print("Not found!");
  }
}

// the loop routine runs over and over again forever
void loop() {
}
