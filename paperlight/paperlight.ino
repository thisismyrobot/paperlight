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

const int minutes_to_ms = 60 * 1000;
const char* host = "papertrailapp.com";
const int port = 443;
const char* latestPath = "/api/v1/events/search.json?limit=1";
String triggerPath = String("/api/v1/events/search.json") + papertrailquery;

void(* resetFunc) (void) = 0;

void setup() {
  M5.begin();
  M5.Axp.ScreenBreath(7);
  pinMode(0, OUTPUT);
}

void loop() {
  while(eventExists()) {
    digitalWrite(0, HIGH);
    delay(30 * minutes_to_ms);
  }

  digitalWrite(0, LOW);
  delay(5 * minutes_to_ms);
  resetFunc();
}

bool eventExists() {

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
  }

  WiFiClientSecure client;

  client.connect(host, port);
  client.println(String("GET ") + latestPath + " HTTP/1.1");
  client.println(String("Host: ") + host);
  client.println(String("X-Papertrail-Token: ") + papertrailtoken);
  client.println();

  String date;
  String line;
  while (client.connected()) {
    line = client.readStringUntil(',');
    if(line.indexOf("min_time_at") > 0) {
      date = line.substring(15, 29);  // YYYY-MM-DDTHH:
      break;
    }
  }
  client.stop();

  client.connect(host, port);
  client.println(String("GET ") + triggerPath + " HTTP/1.1");
  client.println(String("Host: ") + host);
  client.println(String("X-Papertrail-Token: ") + papertrailtoken);
  client.println();

  bool found = false;
  while (client.connected()) {
    line = client.readStringUntil(',');
    if(line.indexOf("generated_at") > 0 && line.indexOf(date) > 0) {
      found = true;
      break;
    }
  }
  client.stop();
  WiFi.disconnect();
  return found;
}
