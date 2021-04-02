// vim: ts=2 sw=2
#pragma once
#include <Esp8266.h>

class WiFiClient
{
	public:
		void write(const void * p, unsigned int length) {};
		bool connected() const { return connected_; }
		bool connect(const char* host, uint16_t port) { return false; };
		void stop() {};
		bool available() { return false; }

		char read() { return 0; };

		operator bool() const { return false; };

	private:
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
