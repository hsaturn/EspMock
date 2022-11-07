/*
  WiFiUdp.h - Library for Arduino Wifi shield.
  Copyright (c) 2011-2014 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified by Ivan Grokhotkov, January 2015 - esp8266 support
  Modified by Brian Park, November 2022 for EpoxyDuino.
*/

#ifndef ESP_MOCK_ESP8266WIFI_WIFIUDP_H
#define ESP_MOCK_ESP8266WIFI_WIFIUDP_H

#include <Udp.h>

/**
 * This is a stub implementation of the WiFiUDP class provided by the ESP8266
 * core at
 * https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/WiFiUdp.h
 */
class WiFiUDP : public UDP {
public:
  WiFiUDP() = default;
  WiFiUDP(const WiFiUDP&) = default;
  WiFiUDP& operator=(const WiFiUDP&) = default;
  virtual ~WiFiUDP() {}

  operator bool() const { return true; }

  uint8_t begin(uint16_t /*port*/) override { return 1; }
  void stop() override {}
  uint8_t beginMulticast(
      IPAddress /*interfaceAddr*/,
      IPAddress /*multicast*/,
      uint16_t /*port*/) {
    return 1;
  }

  int beginPacket(IPAddress /*ip*/, uint16_t /*port*/) override { return 1; }
  int beginPacket(const char * /*host*/, uint16_t /*port*/) override {
    return 1;
  }
  virtual int beginPacketMulticast(
      IPAddress /*multicastAddress*/,
      uint16_t /*port*/,
      IPAddress /*interfaceAddress*/,
      int /*ttl*/ = 1) {
    return 1;
  }
  int endPacket() override { return 1; }
  size_t write(uint8_t) override { return 1; }
  size_t write(const uint8_t * /*buffer*/, size_t size) override {
    return size;
  }

  using Print::write;

  int parsePacket() override { return 1; }
  int available() override { return 1; }
  int read() override { return 1; }
  int read(unsigned char* /*buffer*/, size_t /*len*/) override { return 1; }
  int read(char* buffer, size_t len) override {
    return read((unsigned char*)buffer, len);
  };
  int peek() override { return 1; }
  void flush() override {}

  IPAddress remoteIP() override {return IPAddress(127, 0, 0, 1); }
  uint16_t remotePort() override { return 9999; }
  IPAddress destinationIP() const { return IPAddress(127, 0, 0, 1); }
  uint16_t localPort() const { return 9999; }

  static void stopAll() {}
  static void stopAllExcept(WiFiUDP * /*exC*/) {}
};

#endif // ESP_MOCK_ESP8266WIFI_WIFIUDP_H
