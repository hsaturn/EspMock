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

void start_servers(int n)
{
    while(n)
    {
      ESP8266WiFiClass::selectInstance(n--);
      WiFi.mode(WIFI_STA);
      WiFi.begin("fake_ssid", "fake_pwd");
    }
}

test(wifi_should_connect_on_fake_network)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin("fake_ssid", "fake_pwd");

  assertEqual(WiFi.status(), WL_CONNECTED);
  assertNotEqual(static_cast<uint32_t>(WiFi.localIP()), (uint32_t)0);
}

test(wifi_two_esp_shouldnt_have_same_ip)
{
  start_servers(2);
  ESP8266WiFiClass::selectInstance(1);
  assertEqual(WiFi.status(), WL_CONNECTED);
  auto ip1 = WiFi.localIP().toString();

  ESP8266WiFiClass::selectInstance(2);
  assertEqual(WiFi.status(), WL_CONNECTED);
  auto ip2 = WiFi.localIP().toString();

  assertNotEqual(ip1, ip2);
}

test(wifi_should_be_wldisconnect_and_ip_unset_after_disconnection)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin("fake_ssid", "fake_pwd");
  WiFi.disconnect();
  assertEqual(WiFi.status(), WL_DISCONNECTED);

  // EpoxyDuino has no implementation of IPAddress::isSet()
  // so we assume that 0 is an unset ip
  assertEqual(static_cast<uint32_t>(WiFi.localIP()), (uint32_t)0);
}

test(network_one_esp_client_connects_to_server)
{
  start_servers(1);
  IPAddress ip = WiFi.localIP();

  WiFiServer server(80);
  server.begin();

  WiFiClient client;
  assertFalse(client.connected());
  client.connect(ip, 80);
  assertTrue(client.connected());
}

test(network_one_esp_cannot_connect_to_itself_with_127_0_0_1)
{
  // TODO verify that a real esp cannot connect to 127.0.0.1
  start_servers(1);

  WiFiServer server(80);
  server.begin();

  IPAddress localhost(127,0,0,1);
  WiFiClient client;
  assertFalse(client.connected());
  client.connect(localhost, 80);
  assertFalse(client.connected());
}

test(network_two_esp_client_connects_to_server)
{
  // IPAddress ip{127,0,0,1}; TODO should work on local esp ?
  start_servers(2);
  IPAddress ip = WiFi.localIP();

  WiFiServer server(80);
  server.begin();

  ESP8266WiFiClass::selectInstance(2);
  WiFiClient client;
  assertFalse(client.connected());
  client.connect(ip, 80);
  assertTrue(client.connected());
}

test(network_should_not_be_able_to_connect_to_unstarted_server)
{
  start_servers(1);
  IPAddress ip = WiFi.localIP();
  WiFiServer server(80);
  // --> no server.begin()

  WiFiClient client;
  assertFalse(client.connected());
  client.connect(ip, 80);
  assertFalse(client.connected());
}

test(network_simulate_two_esp_have_different_ip)
{
    start_servers(2);
    ESP8266WiFiClass::selectInstance(1);
    IPAddress ip_1 = WiFi.localIP();

    ESP8266WiFiClass::selectInstance(2);
    IPAddress ip_2 = WiFi.localIP();

    assertNotEqual(ip_1, ip_2);

    // Back to 1st instance
    ESP8266WiFiClass::selectInstance(1);
    IPAddress ip_11 = WiFi.localIP();
    assertEqual(ip_1, ip_11);
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
    ESP8266WiFiClass::resetInstances();
}

void loop() {
  aunit::TestRunner::run();
}
