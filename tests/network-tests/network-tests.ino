// vim: ts=2 sw=2 expandtab
#include <AUnit.h>
#include <ESP8266WiFi.h>

/**
  * EspMock netword unit tests.
  *
  * No wifi connection unit tests.
  * Checks with a local broker. Clients must connect to the local broker
  **/

using namespace std;

test(network_wificlient_should_return_connected_when_connected_byname)
{
    WiFiClient client;
    assertFalse(client.connected());
    client.connect("www.google.com", 80);
    assertTrue(client.connected());
}

test(network_wificlient_should_return_connected_when_connected_byipaddress)
{
  IPAddress ip{127,0,0,1};
  WiFiClient client;
  assertFalse(client.connected());
  client.connect(ip, 80);
  assertTrue(client.connected());
}

test(network_wificlient_should_not_return_connected_when_not_connected)
{
    WiFiClient client;
    assertFalse(client.connected());
    client.connect("cannot-connect-there-because-of-wrong-domain.com", 1234);
    assertFalse(client.connected());
}

void setup() {
}

void loop() {
  aunit::TestRunner::run();
}
