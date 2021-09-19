#include "WiFiServer.h"
#include "WiFiClient.h"
#include "ESP8266WiFi.h"

WiFiServer::WiFiServer(uint16_t port) : _port(port)
{
    wifi = ESP8266WiFiClass::getInstance();
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
      _unclaimed.push_back(client);
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
