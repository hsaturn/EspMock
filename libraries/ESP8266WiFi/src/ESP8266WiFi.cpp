#include "ESP8266WiFi.h"

std::set<uint8_t> ESP8266WiFiClass::ip_used;
std::map<int, std::shared_ptr<ESP8266WiFiClass>> ESP8266WiFiClass::instances;
std::map<IPAddress, std::shared_ptr<ESP8266WiFiClass>> ESP8266WiFiClass::network;
int ESP8266WiFiClass::current_instance = 0;

ESP8266WiFiClass WiFi;

void ESP8266WiFiClass::selectInstance(int n)
{
  if (n == current_instance) return;

  if ((current_instance==0) and (instances.find(0) == instances.end()))
  {
    instances[0] = std::make_shared<ESP8266WiFiClass>();
  }
  *instances[current_instance] = WiFi;

  auto& instance = instances[n];
  if (instance == nullptr)
    instance = std::make_shared<ESP8266WiFiClass>();

  WiFi = *instance;
  
  current_instance = n;
}

ESP8266WiFiClass::ESP8266WiFiClass()
{
  disconnect();
}

bool ESP8266WiFiClass::disconnect(bool wifioff)
{
  if (status_ == WL_CONNECTED)
  {
    uint8_t last = static_cast<uint32_t>(localIP()) & 0xFF;
    ip_used.erase(last);
    network.erase(ip_address_);

    status_ = WL_DISCONNECTED;
    if (wifioff) mode_ = WIFI_OFF;
    ip_last = 0;
    ip_address_ = static_cast<uint32_t>(0);
    return true;
  }
  return false;
}

wl_status_t ESP8266WiFiClass::begin(const char* /* ssid */, const char * /* passphrase */, int32_t /* channel */, const uint8_t* /* bssid */, bool /* connect */)
{
  if (status_ == WL_CONNECTED) disconnect();

  if (mode_ == WIFI_STA)
  {
    uint8_t last=1;
    while(last<255)
    {
      if (ip_used.count(last))
        last++;
      else
      {
        uint8_t ip[4] = {192,168,1,last};
        ip_address_ = ip;
        ip_used.insert(last);
        ip_last = last;
        status_ = WL_CONNECTED;
        break;
      }
    }
  }
  return status_;
}



bool ESP8266WiFiClass::mode(WiFiMode_t m, WiFiState* /* state */)
{
  mode_ = m;
  return true;
}
