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
#include <esp_bt_main.h>
#include <esp_wifi.h>
#include <driver/rtc_io.h>

#include "settings.h"

// Power IC IP5306
#define IP5306_ADDR           117
#define IP5306_REG_SYS_CTL0   0x00
#define IP5306_REG_READ1      0x71

// Papertrail
const char* host = "papertrailapp.com";
const int port = 443;
const char* latestPath = "/api/v1/events/search.json?limit=1";
String triggerPath = String("/api/v1/events/search.json") + papertrailquery;

RTC_DATA_ATTR int sleepsToGo = 0;

void startSleep(int cycles) {
  sleepsToGo = cycles;

  esp_wifi_disconnect();
  esp_wifi_stop();
  
  esp_bluedroid_disable();
  esp_bluedroid_deinit();

  // Disable PowerBoostKeepOn
  Wire.begin(21, 22, 100000);
  Wire.beginTransmission(IP5306_ADDR);
  Wire.write(IP5306_REG_SYS_CTL0);
  Wire.write(0x35);    // 0x37 is default reg value
  Wire.endTransmission();

  // Sleep < 32s regardless due to http://forum.m5stack.com/topic/62/ip5306-automatic-standby
  esp_sleep_enable_timer_wakeup(30 * 1000 * 1000);
  esp_deep_sleep_start();  
}

void setup() {
  if (sleepsToGo > 1) {
    startSleep(sleepsToGo - 1);
  }
  
  M5.begin();
  M5.Axp.ScreenBreath(7);
  pinMode(0, OUTPUT);

  while(eventExists()) {
    digitalWrite(0, HIGH);
    startSleep(60);  // 30 minutes
  }

  digitalWrite(0, LOW);
  startSleep(20);  // 10 minutes
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

void loop() {
}
