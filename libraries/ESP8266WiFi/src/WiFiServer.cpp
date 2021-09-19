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

void WiFiServer::_accept(WiFiClient* client)
{
    _unclaimed.push(client);
}

bool WiFiServer::hasClient()
{
    return _unclaimed.size();
}

WiFiClient WiFiServer::available(byte* /* status */)
{
    if (_unclaimed.size())
    {
        WiFiClient* result = _unclaimed.front();
        return *result;
    }

    return WiFiClient();
}

void WiFiServer::close()
{
    wifi->removeListener(this);
    _unclaimed = std::queue<WiFiClient*>();
    _state = CLOSED;
}

void WiFiServer::stop() {
    close();
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
