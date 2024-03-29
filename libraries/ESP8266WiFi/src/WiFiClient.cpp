#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP8266WiFi.h>
#include <assert.h>

std::set<WiFiClient::Data*> WiFiClient::Data::instances;

std::list<NetworkObserver*> WiFiClient::observers;

NetworkObserver::NetworkObserver(NetworkObserver::callback cb)
: cb_(cb)
{
  WiFiClient::addObserver(this);
}

NetworkObserver::~NetworkObserver()
{
  WiFiClient::unregister(this);
}

WiFiClient::Data::Data()
{
  instances.insert(this);
}

WiFiClient::Data::~Data()
{
  instances.erase(this);
}

void WiFiClient::Data::disconnect(ESP8266WiFiClass* wifi_off)
{
  for(auto data: instances)
  {
    if ((data->wifi.get() == wifi_off)
        or (data->connected_ and data->connected_->data->wifi.get() == wifi_off))
    {
      data->connecting_ = false;
      data->connected_ = nullptr;
    }
  }
}

void WiFiClient::unregister(NetworkObserver* observer)
{
  auto it = std::find(observers.begin(), observers.end(), observer);
  if (it != observers.end())
    observers.erase(it);
}

WiFiClient::WiFiClient()
{
    data = std::make_shared<Data>();
    data->wifi = ESP8266WiFiClass::getInstance();
}

int WiFiClient::connect(const char* sip, uint16_t port)
{
    IPAddress ip;
    ip.fromString(sip);
    data->portno = port;
    return connect(ip, port);
}

uint8_t WiFiClient::connected()
{
  if (data->wifi and data->wifi->status() == WL_CONNECTED)
    return data->connected_ != nullptr;
  return false;
}

void WiFiClient::_close()
{
    if (data->connected_) data->connected_->_establish_link(nullptr);
    // No clear member ...
    while(data->buffer.size()) data->buffer.pop();
}

int WiFiClient::read()
{
    int result = -1;
    if (data->buffer.size())
    {
        result = data->buffer.front();
        data->buffer.pop();
    }
    return result;
}

WiFiClient::WiFiClient(WiFiClient* link, std::shared_ptr<ESP8266WiFiClass> wifi)
{
    data = std::make_shared<Data>();
    data->wifi = wifi;
    data->connected_ = link;
    if (link) link->_establish_link(this);
}

WiFiClient::~WiFiClient()
{
  /** we should don't close since I think Esp lib does not closes too **/
  /* If a day it is decided that it is good to close, then
     unit tests will fail. The data struct is shared between all
     copies of this, so other copies won't be anymore able to communicate */
}

void WiFiClient::_establish_link(WiFiClient* client)
{
    data->connected_ = client;
}

int WiFiClient::connect(IPAddress ip, uint16_t port)
{
    _close();
    data->connected_ = nullptr;
    data->connecting_ = true;
    auto ptr = ESP8266WiFiClass::getInstance(ip);

    // ESP cannot connect to itself
    if (ptr == data->wifi or ptr==nullptr)
    {
        data->connecting_ = false;
        return 0;
    }
    if (not ptr->establishLink(port, this))
        data->connecting_ = false;

    return data->connecting_;
}

void WiFiClient::_close(WiFiServer* /* server */)
{
    data->connecting_ = false;
    // if (server == connected_) connected_ = nullptr;
}

size_t WiFiClient::write(const uint8_t* buffer, size_t length)
{
    if (data->connected_ == nullptr) return 0;
    data->connected_->_incoming(buffer, length);
    for(auto observer: observers)
      observer->notify(this, buffer, length);
    return length;
}

void WiFiClient::_incoming(const uint8_t* in, size_t length)
{
    assert(data);
    for(size_t pos=0; pos<length; pos++)
        data->buffer.push(in[pos]);
}
