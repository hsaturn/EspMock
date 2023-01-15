# See https://github.com/bxparks/EpoxyDuino for documentation about this
# Makefile to compile and run Arduino programs natively on Linux or MacOS.

CXXFLAGS = -Wextra -Wall -std=gnu++11 -fno-exceptions -fno-threadsafe-statics
EXTRA_CXXFLAGS=-g3 -O0

APP_NAME := network-tests
ARDUINO_LIBS := AUnit ESP8266WiFi TinyConsole
ARDUINO_LIB_DIRS := ../../libraries
EPOXY_CORE := EPOXY_CORE_ESP8266
include ../../../EpoxyDuino/EpoxyDuino.mk
