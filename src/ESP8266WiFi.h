// vim: ts=2 sw=2
#pragma once
#include <Esp8266.h>

class WiFiClient
{
	public:
		void write(const char* p, unsigned int length);
		bool connected() const;
		bool connect(const char* host, uint16_t port);
		void stop();
};

class WiFiServer
{
	public:
		WiFiServer(const unsigned int);
		void begin();
		int port() const;
};
