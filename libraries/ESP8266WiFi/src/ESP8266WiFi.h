#pragma once

#include "WiFiClient.h"
#include "ESP8266WiFiTypes.h"
#include <wl_definitions.h>
#include <set>
#include <map>
#include <memory>

class ESP8266WiFiClass
{
  public:
    ESP8266WiFiClass();
    ~ESP8266WiFiClass() { disconnect(true); }

    wl_status_t status() { return status_; }
    wl_status_t begin(const char* ssid, const char *passphrase = NULL, int32_t channel = 0, const uint8_t* bssid = NULL, bool connect = true);
    bool mode(WiFiMode_t, WiFiState* state = nullptr);

    bool disconnect(bool wifioff=false);

    WiFiMode_t getMode() { return mode_; }

    IPAddress localIP() { return ip_address_; }

    static void selectInstance(int n);

  private:
    int ip_last = 0;
    wl_status_t status_;
    WiFiMode_t mode_ = WIFI_OFF;
    IPAddress ip_address_;

    static std::set<uint8_t> ip_used;  // Used IP (*.*.*.x)
    static std::map<int, std::shared_ptr<ESP8266WiFiClass>> instances;
    static int current_instance;
    // static std::shared_ptr<ESP8266WiFiClass> instance;

    static std::map<IPAddress, std::shared_ptr<ESP8266WiFiClass>> network;
};

extern ESP8266WiFiClass WiFi;
