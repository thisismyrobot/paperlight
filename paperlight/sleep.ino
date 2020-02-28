#include <esp_bt_main.h>
#include <esp_wifi.h>
#include <driver/rtc_io.h>

// Power IC IP5306
#define IP5306_ADDR 117
#define IP5306_REG_SYS_CTL0 0x00
#define IP5306_REG_READ1 0x71

RTC_DATA_ATTR int deepSleepsToGo = 0;

void sustainDeepSleep() {
  if (deepSleepsToGo > 1) {
    startDeepSleep(deepSleepsToGo - 1);
  }
}

void deepSleepMinutes(int minutes) {
  startDeepSleep(minutes * 2);
}

void shallowSleepMinutes(int minutes) {
  delay(minutes * 60 * 1000);
  ESP.restart();
}

// Deep (< 10mA) sleep in 30 second cycles.
void startDeepSleep(int cycles) {
  deepSleepsToGo = cycles;

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

