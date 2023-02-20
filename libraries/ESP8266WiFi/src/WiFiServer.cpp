#include "WiFiServer.h"
#include "WiFiClient.h"
#include "ESP8266WiFi.h"

WiFiServer::WiFiServer(uint16_t port) : _port(port)
{
    wifi = ESP8266WiFiClass::getInstance();
    early_accept = ESP8266WiFiClass::earlyAccept;
}

void WiFiServer::earlyAccept(bool early)
{
    if (early == early_accept) return;
    if (early)
    {
        for(WiFiClient* unclaimed: _unclaimed)
        {
            _early_accepted.push_front(new WiFiClient(unclaimed, wifi));
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
          WiFiClient* early = new WiFiClient(client, wifi);
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

WiFiClient WiFiServer::available(byte* status)
{
  { return accept(status); }
}

WiFiClient WiFiServer::accept(byte* /* status */)
{
    WiFiClient retval;

    if (_early_accepted.size())
    {
        WiFiClient* result = _early_accepted.front();
        _early_accepted.pop_front();
        retval = *result; 
        delete result;
    }

    if (_unclaimed.size())
    {
        WiFiClient* client = _unclaimed.front();
        WiFiClient* result = new WiFiClient(client, wifi);

        _unclaimed.pop_front();
        retval = *result;
        delete result;
    }

    return retval;
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
      if (early->data->connected_ == client)
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
