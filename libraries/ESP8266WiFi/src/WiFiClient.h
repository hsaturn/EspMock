// vim: ts=2 sw=2 expandtab
#pragma once
#include <Arduino.h>
#include <IPAddress.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <Client.h>

class WiFiClient : public Client
{
  public:
    uint8_t connected() override { return connected_; }

    int connect(const char* ip, uint16_t port) override;

    int connect(IPAddress ip, uint16_t port) override;

    size_t write(uint8_t) override
    {
      Serial.println("NO IMPLEMENTED write(uint8_t)");
      return 0;
    }
    size_t write(const char* buffer, size_t length)
    { return write((uint8_t*)buffer, length); }

    size_t write(const uint8_t* buffer, size_t length) override
    {
      if (!connected_) return 0;
      ssize_t n = ::write(sockfd, buffer, length);
      if (n < 0) return 0;
      return (size_t)n;
    };
    void flush() override { Serial.println("NYI flush"); };
    int peek() override { Serial.println("NYI peek"); return 0; }
    void stop() {};
    int available() override
    {
      if (!connected()) return 0;
      int value; if (ioctl(sockfd, FIONREAD, &value)==-1) return 0;
      return value;
    }

    operator bool() override { return available() or connected(); }

    int read() { return 0; };

    operator bool() const { return false; };

    virtual ~WiFiClient(){ /* don't close since I think Esp lib does not closes too */ };

  private:
    void close_()
    {
      if (!connected_) return;
      connected_ = false;
    };
    int sockfd, portno;
    bool connected_ = false;
};

