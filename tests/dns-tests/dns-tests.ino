// vim: ts=2 sw=2 expandtab
#include <AUnit.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <string>

/**
  * EspMock netword unit tests.
  *
  * No wifi connection unit tests.
  * Checks with a local broker. Clients must connect to the local broker
  **/

using namespace std;

int i=0;

#define concatn(s,n) ((std::string(s)+std::to_string(n)).c_str())

void start_servers(int n, bool early_accept = false)
{
    ESP8266WiFiClass::resetInstances();
    ESP8266WiFiClass::earlyAccept = early_accept;
    while(n)
    {
      ESP8266WiFiClass::selectInstance(n);
      WiFi.mode(WIFI_STA);
      WiFi.begin(concatn("ssid_", n), concatn("pwd_", n));
      n--;
    }
}

test(begin_true_nowifi)
{
  assertEqual(MDNS.begin("dummy"), true);
}

test(begin_true_with_wifi)
{
  start_servers(1);
  assertEqual(MDNS.begin("dummy"), true);
}

test(hostbyname_one_interface)
{
}

void setup() { ESP8266WiFiClass::resetInstances(); }

void loop() { aunit::TestRunner::run(); }
