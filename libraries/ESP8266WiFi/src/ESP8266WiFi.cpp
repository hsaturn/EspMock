#include "ESP8266WiFi.h"
#include <assert.h>

std::map<int, std::shared_ptr<ESP8266WiFiClass>> ESP8266WiFiClass::instances;
int ESP8266WiFiClass::current_instance = 0;

ESP8266WiFiProxy WiFi;
static ESP8266WiFiClass _WiFi;

void ESP8266WiFiClass::selectInstance(int n)
{
  // instance 0 is the global WiFi instance
  // once we use many instances it becomes unusable
  assert(n);

  if (n == current_instance) return;

  // if (current_instance) *instances[current_instance] = WiFi;

  auto& instance = instances[n];
  if (instance == nullptr)
    instance = std::make_shared<ESP8266WiFiClass>();

  // WiFi = *instance;
  
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
    status_ = WL_DISCONNECTED;
    if (wifioff) mode_ = WIFI_OFF;
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
      IPAddress ip(192,168,1,last);
      if (getInstance(ip) != nullptr)
        last++;
      else
      {
        ip_address_ = ip;
        status_ = WL_CONNECTED;
        break;
      }
    }
  }
  return status_;
}

void ESP8266WiFiClass::resetInstances()
{
  instances.clear();
  current_instance = 0;
}


bool ESP8266WiFiClass::mode(WiFiMode_t m, WiFiState* /* state */)
{
  mode_ = m;
  return true;
}

bool ESP8266WiFiClass::addListener(uint16_t port, WiFiServer* listener)
{
    if (isPortUsed(port)) return false;

    listeners[port] = listener;
    return true;
}

void ESP8266WiFiClass::removeListener(WiFiServer* listener)
{
    for(auto it: listeners)
    {
        if (it.second == listener)
        {
            listeners.erase(it.first);
            break;
        }
    }
}

bool ESP8266WiFiClass::establishLink(uint16_t port, WiFiClient* client)
{
    for(auto it: listeners)
    {
        if (it.first == port)
        {
            it.second->_accept(client);
            return true;
        }
    }
    return false;
}

bool ESP8266WiFiClass::isPortUsed(uint16_t port)
{
    return listeners.find(port) != listeners.end();
}

std::shared_ptr<ESP8266WiFiClass> ESP8266WiFiClass::getInstance(const IPAddress& ip)
{
    for(auto wifi: instances)
        if (wifi.second->ip_address_ == ip) return wifi.second;

    return {};
}

std::shared_ptr<ESP8266WiFiClass> ESP8266WiFiClass::getInstance()
{
    if (current_instance == 0)
    {
        static std::shared_ptr<ESP8266WiFiClass> wifi(&_WiFi, [](ESP8266WiFiClass*){});
        return wifi;
    }
    return instances[current_instance];
}
