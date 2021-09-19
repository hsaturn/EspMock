# EspMock

[![AUnit
Tests](https://github.com/hsaturn/EspMock/actions/workflows/aunit.yml/badge.svg)](https://github.com/hsaturn/EspMock/actions/workflows/aunit.yml)

Mocks for various ESP8266 functions and libraries, designed to be used under
[EpoxyDuino](https://github.com/bxparks/EpoxyDuino) which provides the Arduino
programming API on a Linux, MacOS or FreeBSD environment.

The WiFi simulation can manage many virtual devices so it is possible to write unit tests between many Esps.

This project was originally created to run
[AUnit](https://github.com/bxparks/AUnit) unit tests for the
[TinyMqtt](https://github.com/hsaturn/TinyMqtt) library, but it can be useful
for any other project that needs mocks for ESP8266 and ESP32 libraries.

## Installation

You must install the following projects:

* https://github.com/bxparks/EpoxyDuino
* https://github.com/hsaturn/EspMock
* https://github.com/bxparks/AUnit

These projects *cannot* be installed through the Arduino Library Manager because
they are not normal Arduino libraries. It is probably most convenient to install
them in the same location as other Arduino library directory. In other words,
locate the `./libraries/` directory of your sketchbook directory used by the
Arduino IDE, and run the following commands:

```bash
$ cd {SketchBookDirectory}/libraries
$ git clone https://github.com/bxparks/EpoxyDuino
$ git clone https://github.com/hsaturn/EspMock
$ git clone https://github.com/bxparks/AUnit
```

## Usage

For each Arduino program, we need to create a `Makefile`
as described in [EpoxyDuino](https://github.com/bxparks/EpoxyDuino), but with a
few extra parameters:

* Add the `EPOXY_CORE` variable with the value `EPOXY_CORE_ESP8266`.
* Add a `ARDUINO_LIB_DIRS` variable that points to
  `{EspMockDirectory}/libraries` directory to pickup additional ESP8266 or
  ESP32 mock libraries provided by EspMock.

The result is a `Makefile` that looks like this:

```
APP_NAME := {NameOfSketch}
ARDUINO_LIBS := {Lib1} {Lib2} ...
ARDUINO_LIB_DIRS := {EspMockDirectory}/libraries
EPOXY_CORE := EPOXY_CORE_ESP8266
include {EpoxyDuinoDirectory}/EpoxyDuino.mk
```

See [tests/network-tests/Makefile](tests/network-tests/Makefile) for a concrete
example.

## License

[MIT License](https://opensource.org/licenses/MIT)

## Authors

* https://github.com/hsaturn
* https://github.com/bxparks
