#include "ESP8266WiFi.h"

ESP8266WiFiClass WiFi;

ESP8266WiFiClass::ESP8266WiFiClass()
{
  disconnect();
}

bool ESP8266WiFiClass::disconnect(bool wifioff)
{
  ip_address_ = static_cast<uint32_t>(0);
  status_ = WL_DISCONNECTED;
  if (wifioff) mode_ = WIFI_OFF;
  return true;
}

wl_status_t ESP8266WiFiClass::begin(const char* /* ssid */, const char * /* passphrase */, int32_t /* channel */, const uint8_t* /* bssid */, bool /* connect */)
{
  if (mode_ == WIFI_STA)
  {
    status_ = WL_CONNECTED;
  }
  return status_;
}

bool ESP8266WiFiClass::mode(WiFiMode_t m, WiFiState* /* state */)
{
  mode_ = m;
  return true;
}
