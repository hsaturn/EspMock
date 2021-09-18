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
  assertNotEqual(static_cast<uint32_t>(WiFi.localIP()), (uint32_t)0);
}

test(wifi_two_wifi_shouldnt_have_same_ip)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin("fake_ssid", "fake_pwd");
  assertEqual(WiFi.status(), WL_CONNECTED);

  ESP8266WiFiClass esp2;
  esp2.mode(WIFI_STA);
  esp2.begin("fake_ssid", "fake_pwd");
  assertEqual(esp2.status(), WL_CONNECTED);

  assertNotEqual(WiFi.localIP(), esp2.localIP());

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
  assertTrue(client.connected());
}

test(network_simulate_two_esp_have_different_ip)
{
    ESP8266WiFiClass::selectInstance(1);
    WiFi.mode(WIFI_STA);
    WiFi.begin("fake_ssid", "fake_pwd");
    IPAddress ip_1 = WiFi.localIP();

    ESP8266WiFiClass::selectInstance(2);
    WiFi.mode(WIFI_STA);
    WiFi.begin("fake_ssid", "fake_pwd");
    IPAddress ip_2 = WiFi.localIP();

    assertNotEqual(ip_1, ip_2);

    // Back to 1st instance
    ESP8266WiFiClass::selectInstance(1);
    IPAddress ip_11 = WiFi.localIP();
    assertEqual(ip_1, ip_11);
}

test(network_two_esp_communication)
{
    // 1 Connect two esp
    ESP8266WiFiClass::selectInstance(1);
    WiFi.mode(WIFI_STA);
    WiFi.begin("fake_ssid", "fake_pwd");
    IPAddress ip_1 = WiFi.localIP();

    ESP8266WiFiClass::selectInstance(2);
    WiFi.mode(WIFI_STA);
    WiFi.begin("fake_ssid", "fake_pwd");
    IPAddress ip_2 = WiFi.localIP();

    // Now connect two clients together
    ESP8266WiFiClass::selectInstance(1);
    WiFiClient client;
    client.connect(ip_1, 1234);

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
