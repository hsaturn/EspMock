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

enum class DNSResolveType: uint8_t
{
    DNS_AddrType_IPv4 = 0,  // LWIP_DNS_ADDRTYPE_IPV4 = 0
    DNS_AddrType_IPv6,    // LWIP_DNS_ADDRTYPE_IPV6 = 1
    DNS_AddrType_IPv4_IPv6, // LWIP_DNS_ADDRTYPE_IPV4_IPV6 = 2
    DNS_AddrType_IPv6_IPv4  // LWIP_DNS_ADDRTYPE_IPV6_IPV4 = 3
};

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

    // From ESP8266WiFiGeneric.h, implemented as stubs only.
    int hostByName(const char* aHostname, IPAddress& aResult) {
      return hostByName(aHostname, aResult, 10000);
    }
    int hostByName(
        const char* /*aHostname*/,
        IPAddress& /*aResult*/,
        uint32_t /*timeout_ms*/) {
      return 1;
    }
    int hostByName(
        const char* /*aHostname*/,
        IPAddress& /*aResult*/,
        uint32_t /*timeout_ms*/,
        DNSResolveType /*resolveType*/) {
      return 1;
    }
  public:

    static bool earlyAccept;

  private:
    static void init();

    friend class WiFiServer;
    bool addListener(uint16_t port, WiFiServer*);
    void removeListener(WiFiServer*);

    std::map<uint16_t, WiFiServer*> listeners;
    wl_status_t status_ = WL_IDLE_STATUS;
    WiFiMode_t mode_ = WIFI_OFF;
    IPAddress ip_address_;

    using Container = std::map<int, std::shared_ptr<ESP8266WiFiClass>>;

    static std::unique_ptr<Container> instances;
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

    int hostByName(const char* aHostname, IPAddress& aResult) {
      return wifi()->hostByName(aHostname, aResult);
    }
    int hostByName(
        const char* aHostname,
        IPAddress& aResult,
        uint32_t timeout_ms) {
      return wifi()->hostByName(aHostname, aResult, timeout_ms);
    }
    int hostByName(
        const char* aHostname,
        IPAddress& aResult,
        uint32_t timeout_ms,
        DNSResolveType resolveType) {
      return wifi()->hostByName(aHostname, aResult, timeout_ms, resolveType);
    }

    private:
      std::shared_ptr<ESP8266WiFiClass> wifi() const { return ESP8266WiFiClass::getInstance(); }
};
extern ESP8266WiFiProxy WiFi;
