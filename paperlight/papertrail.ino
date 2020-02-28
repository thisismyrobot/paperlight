#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "papertrail.h"
#include "settings.h"

const char* host = "papertrailapp.com";
const int port = 443;
const char* latestPath = "/api/v1/events/search.json?limit=1";
String triggerPath = String("/api/v1/events/search.json?limit=1&q=") + papertrailsearch;

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

  // Get the date of the latest event, using part of that date to filter
  // the search for your particular event.
  String date;
  String line;
  while (client.connected()) {
    line = client.readStringUntil(',');
    if(line.indexOf("min_time_at") > 0) {
      date = line.substring(MASK_START, MASK_START + match_window);
      break;
    }
  }
  client.stop();

  // Look for an event based on your search, filtering event
  // dates to those that match the partial date.
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

