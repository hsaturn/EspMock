#pragma once

#include "WiFiClient.h"
#include "ESP8266WiFiTypes.h"
#include <wl_definitions.h>

class ESP8266WiFiClass
{
  public:
    wl_status_t status() { return status_; }
    wl_status_t begin(const char* ssid, const char *passphrase = NULL, int32_t channel = 0, const uint8_t* bssid = NULL, bool connect = true);
    bool mode(WiFiMode_t, WiFiState* state = nullptr);

    WiFiMode_t getMode() { return mode_; }

  private:
    wl_status_t status_ = WL_DISCONNECTED;
    WiFiMode_t mode_ = WIFI_OFF;
};

extern ESP8266WiFiClass WiFi;
