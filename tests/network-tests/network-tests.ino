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
      WiFi.begin(concatn("ssid_",n), concatn("pwd_", n));
      n--;
    }
}

test(wifi_should_connect_on_fake_network)
{
  start_servers(0);
  WiFi.mode(WIFI_STA);
  WiFi.begin("fake_ssid", "fake_pwd");

  assertEqual(WiFi.status(), WL_CONNECTED);
  assertNotEqual(static_cast<uint32_t>(WiFi.localIP()), (uint32_t)0);
}

test(wifi_static_instance_is_part_of_map)
{
  start_servers(0);
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
    WiFiServer server2(80);
    server2.begin();
    assertEqual(server2.status(), LISTEN);
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
  start_servers(0);
  WiFi.mode(WIFI_STA);
  WiFi.begin("fake_ssid", "fake_pwd");
  WiFi.disconnect();
  assertEqual(WiFi.status(), WL_DISCONNECTED);

  // EpoxyDuino has no implementation of IPAddress::isSet()
  // so we assume that 0 is an unset ip
  assertEqual(static_cast<uint32_t>(WiFi.localIP()), (uint32_t)0);
}

test(network_two_esp_global_early_accept)
{
  start_servers(2, true);
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

test(network_two_esp_send_and_receive_bytes)
{
  start_servers(2, true);
  IPAddress ip = WiFi.localIP();

  WiFiServer server(80);
  server.begin();

  ESP8266WiFiClass::selectInstance(2);
  WiFiClient client;
  assertFalse(client.connected());
  client.connect(ip, 80);

  assertTrue(client.connected());

  WiFiClient link = server.available();
  assertTrue(link.connected());

  std::string sent ="abcd";
  client.write(&sent[0], sent.size());

  assertTrue(link.available());

  std::string received;
  while(link.available())
      received += link.read();

  assertEqual(sent.c_str(), received.c_str());
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
  assertTrue(link.connected());
}

test(network_two_esp_available_with_late_early_connect)
{
  start_servers(2, false);
  IPAddress ip = WiFi.localIP();

  WiFiServer server(80);
  server.begin();

  ESP8266WiFiClass::selectInstance(2);
  WiFiClient client;
  client.connect(ip, 80);

  // Link not established yet
  assertFalse(client.connected());
  server.earlyAccept(true);

  // establish link
  WiFiClient link = server.available();
  assertTrue(link.connected());
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

test(write_observer)
{
  char buffer[128];
  size_t buffsize;
  start_servers(2, true);
  IPAddress ip = WiFi.localIP();

  WiFiServer server(80);
  server.begin();
  server.earlyAccept(true);

  ESP8266WiFiClass::selectInstance(2);
  WiFiClient client;
  client.connect(ip, 80);
  assertTrue(client.connected());

  NetworkObserver observer([&buffer, &buffsize](const WiFiClient*, const uint8_t* incoming, size_t length)
  {
    memcpy(buffer, incoming, length);
    buffsize = length;
  });

  const char* sent("abcd");
  client.write(sent, 4);
  assertEqual(4, (int)buffsize);
  assertEqual(memcmp(sent, buffer, 4), 0);
}

void setup()
{
    ESP8266WiFiClass::resetInstances();
}

void loop() {
  aunit::TestRunner::run();
}
