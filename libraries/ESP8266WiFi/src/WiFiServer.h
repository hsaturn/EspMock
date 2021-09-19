#pragma once

#include <wl_definitions.h>

#include <queue>
#include <memory>

class WiFiClient;
class ESP8266WiFiClass;

class WiFiClient;

class WiFiServer
{
public:
  WiFiServer(uint16_t port);
  virtual ~WiFiServer() { close(); }
  WiFiClient available(uint8_t* status = NULL);
  bool hasClient();
  void begin(uint16_t port = 0, uint8_t backlog = 0);
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  uint8_t status() { return _state; } // -> tcp_state
  uint16_t port() const { return _port; }
  void close();
  void stop();

  // emulation
  void _accept(WiFiClient*);

private:
  std::queue<WiFiClient*> _unclaimed;
  uint16_t _port;
  std::shared_ptr<ESP8266WiFiClass> wifi;
  uint8_t _state = CLOSED;
};
