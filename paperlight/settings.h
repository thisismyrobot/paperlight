#ifndef settings_h
#define settings_h

#include "papertrail.h"

// Your WiFi settings.
const char* ssid = "your-ssid";
const char* password = "your-password";

// You Papertrail API settings.
// The API key is under https://papertrailapp.com/account/profile
//
// https://help.papertrailapp.com/kb/how-it-works/search-api/
//
// The search is the query you're running on your logs to find the event.
const char* papertrailtoken = "replace-with-token";
const char* papertrailsearch = "replace-with-search";

// Alert behaviour settings.
const int match_window = SAME_HOUR;  // Window of time do you want to alert in - aka SAME_MINUTE means alert if event seen in *this* minute.
const int stay_on_minutes = 60;  // Once there is an alert, how long to illuminate for before re-checking.
const int re_check_minutes = 30;  // Once there is no alert, how often to re-check.

#endif

