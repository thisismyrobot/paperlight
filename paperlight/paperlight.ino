/*
 * Add https://dl.espressif.com/dl/package_esp32_index.json as an additional board manager URL.
 * Install "esp32" board via the board manager.
 * Install the "M5StickC 0.1.1" library via Manage Libraries.
 * Select "M5Stick-C" in the boards manager.
 * Select 115200 as the upload speed.
 */
#include <M5StickC.h>

#include "settings.h"

#define RELAY_GPIO 26
#define RELAY_ON 1
#define RELAY_OFF 0

void setup() {
  // Sustain deep sleep until full duration complete.
  sustainDeepSleep();

  M5.begin();
  M5.Axp.ScreenBreath(7);
  pinMode(RELAY_GPIO, OUTPUT);

  while(eventExists()) {
    digitalWrite(RELAY_GPIO, RELAY_ON);
    shallowSleepMinutes(stay_on_minutes);
  }

  digitalWrite(RELAY_GPIO, RELAY_OFF);
  deepSleepMinutes(re_check_minutes);
}

void loop() {
}
