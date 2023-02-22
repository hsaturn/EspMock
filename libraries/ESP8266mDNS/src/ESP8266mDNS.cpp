#include "ESP8266mDNS.h"

// ESP8266WiFi
#include <ESP8266WiFi.h>

MDNSResponder MDNS;

MDNSResponder::MDNSResponder() {}

MDNSResponder::~MDNSResponder() {}

bool MDNSResponder::begin(const char* hostname)
{
  auto instance = ESP8266WiFiClass::getInstance();
  if (instance)
    instance->_setHostName(hostname);
  return true;
}
