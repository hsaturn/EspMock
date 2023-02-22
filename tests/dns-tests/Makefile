# See https://github.com/bxparks/EpoxyDuino for documentation about this
# Makefile to compile and run Arduino programs natively on Linux or MacOS.

CXXFLAGS = -Wextra -Wall -std=c++17 -fno-exceptions -fno-threadsafe-statics
EXTRA_CXXFLAGS=-g3 -O0

APP_NAME := dns-tests
ARDUINO_LIBS := AUnit ESP8266WiFi TinyConsole ESP8266mDNS
ARDUINO_LIB_DIRS := ../../libraries
EPOXY_CORE := EPOXY_CORE_ESP8266

include ../next.mk
