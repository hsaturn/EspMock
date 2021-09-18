#pragma once

#include "WiFiClient.h"
#include "ESP8266WiFiTypes.h"
#include <wl_definitions.h>

class ESP8266WiFiClass
{
  public:
    ESP8266WiFiClass();

    wl_status_t status() { return status_; }
    wl_status_t begin(const char* ssid, const char *passphrase = NULL, int32_t channel = 0, const uint8_t* bssid = NULL, bool connect = true);
    bool mode(WiFiMode_t, WiFiState* state = nullptr);

    bool disconnect(bool wifioff=false);

    WiFiMode_t getMode() { return mode_; }

    IPAddress localIP() { return ip_address_; }

  private:
    wl_status_t status_;
    WiFiMode_t mode_ = WIFI_OFF;
    IPAddress ip_address_;
};

extern ESP8266WiFiClass WiFi;
