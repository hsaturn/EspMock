#pragma once

#include <Arduino.h>
#include <Stream.h>

class _FLOAT
{
	public:
		int digits;
		float val;
};

class Esp8266
{
	public:
		void reset() {};
};

class Serial_ : public Stream
{
};

Esp8266 ESP;
