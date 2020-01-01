/*
 * Add https://dl.espressif.com/dl/package_esp32_index.json as an additional board manager URL.
 * Install "esp32" board via the board manager.
 * Install the "M5StickC 0.1.1" library via Manage Libraries.
 * Select "M5Stick-C" in the boards manager.
 * Select 115200 as the upload speed.
 */
#include <M5StickC.h>
#include <WiFi.h>

#include "settings.h"

void setup(){
    M5.begin();

    M5.Lcd.print("Hello World");

    WiFi.begin(ssid, password);

    Serial.begin(115200);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
}

// the loop routine runs over and over again forever
void loop() {
}
