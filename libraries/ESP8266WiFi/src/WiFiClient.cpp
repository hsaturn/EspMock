#include <WiFiClient.h>
#include <ESP8266WiFi.h>

int WiFiClient::connect(const char* sip, uint16_t port)
{
    IPAddress ip;
    ip.fromString(sip);
    return connect(ip, port);
}

int WiFiClient::connect(IPAddress ip, uint16_t port)
{
    connected_ = false;
    auto ptr = ESP8266WiFiClass::getInstance(ip);
    if (ptr == nullptr) return 0;
    if (ptr->establishLink(port, this))
        connected_ = true;
    return connected_;
}
