#include "ESP8266WiFi.h"
#include <assert.h>
#include <string>
#include <algorithm>

std::unique_ptr<ESP8266WiFiClass::Container>  ESP8266WiFiClass::instances{};
int ESP8266WiFiClass::current_instance = 0;
bool ESP8266WiFiClass::earlyAccept = false;
std::map<IPAddress, std::string> ESP8266WiFiClass::_registrar;

ESP8266WiFiProxy WiFi;

ESP8266WiFiClass::~ESP8266WiFiClass()
{
  disconnect(true);
  auto it = _registrar.find(ip_address_);
  if (it != _registrar.end())
    _registrar.erase(it);
}

void ESP8266WiFiClass::_setHostName(const char* hostname)
{
  _registrar[ip_address_] = hostname;
}

void ESP8266WiFiClass::init()
{
  if (not instances)
  {
    instances = std::unique_ptr<Container>(new Container);
    current_instance = 0;
    earlyAccept = false;
  }
}

void ESP8266WiFiClass::selectInstance(int n)
{
  init();
  assert(n);
  assert(instances);
  if (n == current_instance) return;

  if (instances->find(n) == instances->end())
      (*instances)[n] = std::make_shared<ESP8266WiFiClass>();

  current_instance = n;
}

ESP8266WiFiClass::ESP8266WiFiClass()
  : status_(WL_IDLE_STATUS)
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
    WiFiClient::Data::disconnect(this);
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
  init();
  instances->clear();
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

WiFiServer* ESP8266WiFiClass::establishLink(uint16_t port, WiFiClient* client)
{
    for(auto it: listeners)
    {
        if (it.first == port and it.second->_accept(client))
        {
            return it.second;
        }
    }
    return nullptr;
}

bool ESP8266WiFiClass::isPortUsed(uint16_t port)
{
    return listeners.find(port) != listeners.end();
}

std::shared_ptr<ESP8266WiFiClass> ESP8266WiFiClass::getInstance(const IPAddress& ip)
{
    init();
    for(auto wifi: *instances)
        if (wifi.second->ip_address_ == ip) return wifi.second;

    return {};
}

std::shared_ptr<ESP8266WiFiClass> ESP8266WiFiClass::getInstance()
{
    init();
    if (current_instance == 0 and instances->find(0) == instances->end())
        (*instances)[0] = std::make_shared<ESP8266WiFiClass>();
    assert(current_instance >= 0);
    return (*instances)[current_instance];
}

int ESP8266WiFiClass::hostByName(
        const char* aHostname,
        IPAddress& aResult,
        uint32_t /* timeout_ms */,
        DNSResolveType resolveType)
{
  if (resolveType != DNSResolveType::DNS_AddrType_IPv4) return 0;
  for(auto it: _registrar)
  {
    if (it.second == aHostname)
    {
      aResult = it.first;
      return 1;
    }
  }
  return 0;
}
