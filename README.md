# EspMock

[![AUnit
Tests](https://github.com/hsaturn/EspMock/actions/workflows/aunit.yml/badge.svg)](https://github.com/hsaturn/EspMock/actions/workflows/aunit.yml)

Mocks for various ESP8266 functions and libraries, designed to be used under
[EpoxyDuino](https://github.com/bxparks/EpoxyDuino) which provides the Arduino
programming API on a Linux, MacOS or FreeBSD environment.

This project was originally created to run
[AUnit](https://github.com/bxparks/AUnit) unit tests for the
[TinyMqtt](https://github.com/hsaturn/TinyMqtt) library, but it can be useful
for any other project that needs mocks for ESP8266 and ESP32 libraries.

## Installation

This project *cannot* be installed through the Arduino Library Manager because
it is compatible only with EpoxyDuino. You need to run the `git clone` command
manually. It is probably best to install at the same location as a normal
Arduino library. In other words, locate the `./libraries/` directory of your
sketchbook directory used by the Arduino IDE:

```bash
$ cd {sketchbook_location}/libraries
$ git clone https://github.com/hsaturn/EspMock
```

Then add `EspMock` to the `ARDUINO_LIBS` variable of the EpoxyDuino `Makefile`
located in directory of the Arduino sketch that you want to compile, like this:

```
APP_NAME := network-tests
ARDUINO_LIBS := {Lib1} {Lib2} ... EspMock
include ../../../EpoxyDuino/EpoxyDuino.mk
```

See [EpoxyDuino](https://github.com/bxparks/EpoxyDuino) for information on how
to configure the `Makefile`.

## License

[MIT License](https://opensource.org/licenses/MIT)

## Authors

* Created by https://github.com/hsaturn
* With help from https://github.com/bxparks
