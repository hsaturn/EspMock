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

    int connect(const char* host, uint16_t port) override
    {
      struct hostent* server;
      if (connected()) close_();
      portno = port;
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd <0) return false;
      server = gethostbyname(host);
      if (server == nullptr) return false;

      struct sockaddr_in serv_addr;
      bzero((char*)&serv_addr, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      bcopy((char*) server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length);
      serv_addr.sin_port = htons(portno);
      if (::connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
        return false;
      connected_ = true;
      return true;
    };

    int connect(IPAddress ip, uint16_t port) override
    {
      if (connected()) close_();
      portno = port;
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd <0) return false;

      struct sockaddr_in serv_addr;
      bzero((char*)&serv_addr, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_port = htons(portno);
      serv_addr.sin_addr.s_addr = (uint32_t)ip;
      if (::connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
        return false;
      connected_ = true;
      return true;
    }
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

class WiFiServer
{
  public:
    WiFiServer(const unsigned int) {};
    void begin() {};
    int port() const { return port_; };
    void close(){};
    WiFiClient available() { return WiFiClient(); }

  private:
    int port_;
};
