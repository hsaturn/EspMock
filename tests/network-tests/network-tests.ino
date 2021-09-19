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
    ESP8266WiFiClass::resetInstances();
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

test(wifi_static_instance_is_part_of_map)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin("fake_ssid", "fake_pwd");

    auto wifi = ESP8266WiFiClass::getInstance(WiFi.localIP());
    assertTrue(wifi != nullptr);
}

test(network_one_esp_cannot_bind_same_port_twice)
{
    start_servers(1);
    WiFiServer server(80);
    server.begin();
    assertEqual(server.status(), LISTEN);

    WiFiServer wrong(80);
    wrong.begin();
    assertNotEqual(wrong.status(), LISTEN);
}

test(network_two_esp_can_bind_same_port_twice)
{
    start_servers(2);
    WiFiServer server(80);
    server.begin();
    assertEqual(server.status(), LISTEN);

    ESP8266WiFiClass::selectInstance(2);
    WiFiServer wrong(80);
    wrong.begin();
    assertEqual(wrong.status(), LISTEN);
}

test(network_one_esp_server_do_not_listen_after_close)
{
    start_servers(1);
    WiFiServer server(80);
    server.begin();
    assertEqual(server.status(), LISTEN);
    server.stop();
    assertNotEqual(server.status(), LISTEN);
}

test(network_two_esp_shouldnt_have_same_ip)
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

test(network_two_esp_server_early_accept)
{
  start_servers(2);
  IPAddress ip = WiFi.localIP();

  WiFiServer server(80);
  server.begin();
  server.earlyAccept(true);

  ESP8266WiFiClass::selectInstance(2);
  WiFiClient client;
  assertFalse(client.connected());
  client.connect(ip, 80);

  // Link already established with early_connect
  assertTrue(client.connected());
}

test(network_two_esp_client_connects_to_server)
{
  start_servers(2);
  IPAddress ip = WiFi.localIP();

  WiFiServer server(80);
  server.begin();

  ESP8266WiFiClass::selectInstance(2);
  WiFiClient client;
  assertFalse(client.connected());
  client.connect(ip, 80);

  // Link not established yet
  assertFalse(client.connected());

  // establish link
  WiFiClient link = server.available();
  assertTrue(client.connected());
}

test(network_one_esp_cannot_connect_to_itself_with_127_0_0_1)
{
  start_servers(1);

  WiFiServer server(80);
  server.begin();

  WiFiClient client;
  assertFalse(client.connected());
  client.connect(WiFi.localIP(), 80);
  assertFalse(client.connected());
}

test(network_one_esp_cannot_connect_to_itself_with_its_ip)
{
  start_servers(1);
  IPAddress ip = WiFi.localIP();

  WiFiServer server(80);
  server.begin();

  WiFiClient client;
  assertFalse(client.connected());
  client.connect(ip, 80);

  // Link not established yet
  assertFalse(client.connected());

  // Establish link
  server.available();
  // But ESP cannot connect to itself so...
  assertFalse(client.connected());
}

test(network_should_not_be_able_to_connect_to_unstarted_server)
{
  start_servers(2);
  IPAddress ip = WiFi.localIP();
  WiFiServer server(80);
  // --> no server.begin()

  ESP8266WiFiClass::selectInstance(2);
  WiFiClient client;
  assertFalse(client.connected());
  client.connect(ip, 80);
  server.available();
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

void setup()
{
    ESP8266WiFiClass::resetInstances();
}

void loop() {
  aunit::TestRunner::run();
}
