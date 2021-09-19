#pragma once

#include <wl_definitions.h>

#include <list>
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

  void earlyAccept(bool early);

  // emulation
private:
  friend class WiFiClient;
  friend class ESP8266WiFiClass;

  bool _accept(WiFiClient*);
  void _close(WiFiClient*); // client destroyed or closes link

private:
  std::shared_ptr<ESP8266WiFiClass> wifi;

  std::list<WiFiClient*> _early_accepted;
  std::list<WiFiClient*> _unclaimed;
  uint16_t _port = 0;
  uint8_t _state = CLOSED;
  WiFiClient* established_ = nullptr;

  // Early accept incoming connection, allows to connect immediately
  // a WiFiClient to a WiFiServer in monothread context. Without that
  // the WiFiClient::connect, can never connect because ::available() is
  // not called.
  bool early_accept = false;
};
