// vim: ts=2 sw=2
#pragma once
#include <Esp8266.h>

class WiFiClient
{
	public:
		void write(const char* p, unsigned int length);
};

class WiFiServer
{
	public:
		void begin();
		int port();
};
