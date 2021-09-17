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

int i=0;

test(wifi_should_connect_on_fake_network)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin("fake_ssid", "fake_pwd");

  assertEqual(WiFi.status(), WL_CONNECTED);
}

test(network_wificlient_should_return_connected_when_connected_byname)
{
    WiFiClient client;
    assertFalse(client.connected());
    client.connect("www.google.com", 80);
    assertTrue(client.connected());
}

test(network_wificlient_should_return_connected_when_connected_byipaddress)
{
  // TODO this test is wrong because localhost does not always have a
  // web server...We should create one before
  IPAddress ip{127,0,0,1};
  WiFiClient client;
  assertFalse(client.connected());
  client.connect(ip, 80);
  // assertTrue(client.connected());
  Serial.println("FIXME this test does not work well");
}

test(network_wificlient_should_not_return_connected_when_not_connected)
{
    WiFiClient client;
    assertFalse(client.connected());
    client.connect("cannot-connect-there-because-of-wrong-domain.com", 1234);
    assertFalse(client.connected());
}

void setup()
{
  i=0;
}

void loop() {
  aunit::TestRunner::run();
}
