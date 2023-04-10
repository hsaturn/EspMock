// vim: ts=2 sw=2 expandtab
#pragma once
#include <Arduino.h>
#include <IPAddress.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <Client.h>
#include <queue>
#include <memory>
#include <functional>
#include <list>
#include <set>

class WiFiServer;
class WiFiClient;
class ESP8266WiFiClass;

class NetworkObserver
{
  public:
    using callback = std::function<void(const WiFiClient*, const uint8_t* buffer, size_t length)>;
    NetworkObserver(callback);
    ~NetworkObserver();

    void notify(const WiFiClient* client, const uint8_t* buffer, size_t length)
    {
      cb_(client, buffer, length);
    }

  private:
    callback cb_;
};

class WiFiClient : public Client
{
  public:
    uint8_t connected() override;

    int connect(const char* ip, uint16_t port) override;

    int connect(IPAddress ip, uint16_t port) override;

    size_t write(uint8_t) override
    {
      Serial.println("NO IMPLEMENTED write(uint8_t)");
      return 0;
    }
    size_t write(const char* buffer, size_t length)
    { return write((uint8_t*)buffer, length); }

    size_t write(const uint8_t* buffer, size_t length) override;
    void flush() override { Serial.println("NYI flush"); };
    int peek() override { Serial.println("NYI peek"); return 0; }
    void stop() override { _close(); };
    int available() override { return data->buffer.size(); }

    operator bool() override { return available() or connected(); }

    int read() override;

    operator bool() const { return false; };

    WiFiClient();
    virtual ~WiFiClient();

    static void addObserver(NetworkObserver* observer) { observers.push_back(observer); }

  private:
    friend class WiFiServer;
    friend class NetworkObserver;
    friend class ESP8266WiFiClass;

    WiFiClient(WiFiClient* link, std::shared_ptr<ESP8266WiFiClass> wifi);
  
    void _incoming(const uint8_t* buffer, size_t length);
    void _establish_link(WiFiClient* conn);

    void _close(WiFiServer*); // server destroyed or closes link
    void _close();

    static void unregister(NetworkObserver*);

  private:

    struct Data
    {
      Data();
      ~Data();
      std::shared_ptr<ESP8266WiFiClass> wifi;

      int portno=0;
      WiFiClient* connected_ = nullptr;
      bool connecting_ = false;
      std::queue<uint8_t> buffer{};

      static void disconnect(ESP8266WiFiClass*);
      static std::set<Data*> instances;
    };
    std::shared_ptr<Data> data;

    static std::list<NetworkObserver*> observers;
};

