#include "WiFiServer.h"
#include "WiFiClient.h"
#include "ESP8266WiFi.h"

WiFiServer::WiFiServer(uint16_t port) : _port(port)
{
    wifi = ESP8266WiFiClass::getInstance();
}

void WiFiServer::earlyAccept(bool early)
{
    if (early == early_accept) return;
    if (early)
    {
        for(WiFiClient* unclaimed: _unclaimed)
        {
            WiFiClient* early_client = new WiFiClient;
            early_client->_connected(unclaimed);
            unclaimed->_connected(early_client);
        }
        _unclaimed.clear();
    }
    early_accept = early;
}

void WiFiServer::begin(uint16_t port, uint8_t /* backlog */)
{
    if (port == 0) port = _port;
    close();
    _port = port;

    if (wifi->addListener(_port, this))
    {
        _state = LISTEN;
    }
}

bool WiFiServer::_accept(WiFiClient* client)
{
    if (_state == LISTEN or _state == ESTABLISHED)
    {
      if (early_accept)
      {
          WiFiClient* early = new WiFiClient;
          early->_connected(client);
          client->_connected(early);
          _early_accepted.push_back(early);
      }
      else
      {
          _unclaimed.push_back(client);
      }
      return true;
    }
    return false;
}

bool WiFiServer::hasClient()
{
    return _unclaimed.size();
}

WiFiClient WiFiServer::available(byte* /* status */)
{
    if (_early_accepted.size())
    {
        WiFiClient* result = _early_accepted.front();
        _early_accepted.pop_front();
        return *result;
    }

    if (_unclaimed.size())
    {
        WiFiClient* client = _unclaimed.front();
        WiFiClient* result = new WiFiClient;
        client->_connected(result);
        result->_connected(client);

        _unclaimed.pop_front();
        return *result;
    }

    return WiFiClient();
}

void WiFiServer::close()
{
    wifi->removeListener(this);
    _unclaimed.clear();
    for(WiFiClient* client: _early_accepted)
        delete client;

    _early_accepted.clear();

    _state = CLOSED;
    if (established_) delete established_;
    established_ = nullptr;
}

void WiFiServer::stop()
{
    close();
}

void WiFiServer::_close(WiFiClient* client)
{
  _state = LISTEN;
  _unclaimed.remove(client);
  for(WiFiClient* early: _early_accepted)
  {
      if (early->connected_ == client)
      {
          _early_accepted.remove(early);
          delete early;
      }
  }
}

size_t WiFiServer::write(uint8_t b) {
    return write(&b, 1);
}

size_t WiFiServer::write(const uint8_t *buffer, size_t size) {
    // write to all clients
    // not implemented (from original API)
    (void) buffer;
    (void) size;
    return 0;
}
