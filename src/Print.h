#pragma once
#include <stdint.h>

class Print
{
	public:
		virtual size_t write(uint8_t) = 0;
};
