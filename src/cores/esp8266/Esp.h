#pragma once

#include <Arduino.h>
#include <Stream.h>
#include <sys/time.h>

class _FLOAT
{
	public:
		int digits;
		float val;
};

class EspClass
{
	public:
		EspClass()
		{ gettimeofday(&start, NULL); }
		void reset() {};
		unsigned long getFreeHeap()
		// Very ugly approximation, this is freeStack
		{ int i; static int* h=40000+&i; return h-&i; }

		uint32_t getCpuFreqMHZ() { return 80; }
		uint32_t getCycleCount() {
			struct timeval now;
			gettimeofday(&now, NULL);
			return getCpuFreqMHZ()*((now.tv_sec-start.tv_sec)*1000000+(now.tv_usec-start.tv_usec));
		}

	private:
		struct timeval start;
};

class Serial_ : public Stream
{
};

extern EspClass ESP;
