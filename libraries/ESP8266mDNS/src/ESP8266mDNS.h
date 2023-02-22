#pragma once
// @see https://raw.githubusercontent.com/espressif/arduino-esp32/master/libraries/ESPmDNS/src/ESPmDNS.h

#include "Arduino.h"
#include <map>
#include <string>

// ESP8266WiFi.h
#include <IPAddress.h>

class MDNSResponder
{
public:
  MDNSResponder();
  ~MDNSResponder();

  bool begin(const char* hostName);


  
private:

};

extern MDNSResponder MDNS;

