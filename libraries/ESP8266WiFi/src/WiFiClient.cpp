#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP8266WiFi.h>

WiFiClient::WiFiClient()
{
    wifi = ESP8266WiFiClass::getInstance();
}

int WiFiClient::connect(const char* sip, uint16_t port)
{
    IPAddress ip;
    ip.fromString(sip);
    portno = port;
    return connect(ip, port);
}

void WiFiClient::_close()
{
    if (connected_) connected_->_connected(nullptr);
    connected_ = nullptr;
    buffer = decltype(buffer)();
}

int WiFiClient::read()
{
    int result = -1;
    if (buffer.size())
    {
        result = buffer.front();
        buffer.pop();
    }
    return result;
}

WiFiClient::~WiFiClient()
{
  /** we should don't close since I think Esp lib does not closes too
  but if we do that, we may encounter bad behavious **/
  _close();
}

void WiFiClient::_connected(WiFiClient* client)
{
    if (connecting_ == false) return;
    connecting_ = false;
    _close();
    connected_ = client;
}

int WiFiClient::connect(IPAddress ip, uint16_t port)
{
    _close();
    connected_ = nullptr;
    connecting_ = true;
    auto ptr = ESP8266WiFiClass::getInstance(ip);

    // ESP cannot connect to itself
    if (ptr == wifi or ptr==nullptr)
    {
        connecting_ = false;
        return 0;
    }
    if (not ptr->establishLink(port, this))
        connecting_ = false;

    return connecting_;
}

void WiFiClient::_close(WiFiServer* /* server */)
{
    connecting_ = false;
    // if (server == connected_) connected_ = nullptr;
}

size_t WiFiClient::write(const uint8_t* buffer, size_t length)
{
    if (connected_ == nullptr) return 0;
    connected_->_incoming(buffer, length);
    return length;
}

void WiFiClient::_incoming(const uint8_t* in, size_t length)
{
    for(size_t pos=0; pos<length; pos++)
        buffer.push(in[pos]);
}
