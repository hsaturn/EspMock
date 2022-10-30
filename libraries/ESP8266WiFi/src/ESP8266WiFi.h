#pragma once

#include "WiFiClient.h"
#include "WiFiServer.h"
#include <wl_definitions.h>
#include <IPAddress.h>
#include "ESP8266WiFiTypes.h"
#include <set>
#include <map>
#include <memory>

class WiFiServer;


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

    // Multipe ESP Emulation
    static void selectInstance(int n);  // n>0 !
    static void resetInstances();

    static std::shared_ptr<ESP8266WiFiClass> getInstance(const IPAddress& ip);
    static std::shared_ptr<ESP8266WiFiClass> getInstance();
    static int getInstanceNumber() { return current_instance; };

    bool isPortUsed(uint16_t port);
    WiFiServer* establishLink(uint16_t port, WiFiClient*);

    static bool earlyAccept;

  private:

    friend class WiFiServer;
    bool addListener(uint16_t port, WiFiServer*);
    void removeListener(WiFiServer*);

    std::map<uint16_t, WiFiServer*> listeners;
    wl_status_t status_;
    WiFiMode_t mode_ = WIFI_OFF;
    IPAddress ip_address_;

    static std::map<int, std::shared_ptr<ESP8266WiFiClass>> instances;
    static int current_instance;
};

class ESP8266WiFiProxy
{
  public:
    wl_status_t status() { return wifi()->status(); }
    wl_status_t begin(const char* ssid, const char *passphrase = NULL, int32_t channel = 0, const uint8_t* bssid = NULL, bool connect = true) { return wifi()->begin(ssid, passphrase, channel, bssid, connect); }
    bool mode(WiFiMode_t m, WiFiState* state = nullptr) { return wifi()->mode(m,state); }
    bool disconnect(bool wifioff=false) { return wifi()->disconnect(wifioff); }
    WiFiMode_t getMode() { return wifi()->getMode(); }
    IPAddress localIP() { return wifi()->localIP(); }

    private:
      std::shared_ptr<ESP8266WiFiClass> wifi() const { return ESP8266WiFiClass::getInstance(); }
};
extern ESP8266WiFiProxy WiFi;
