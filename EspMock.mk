# vim: set noexpandtab
espmock_path := $(abspath $(lastword $(MAKEFILE_LIST)))
espmock_dir := $(dir $(espmock_path))

ESP_MODEL = esp8266
ESPMOCK_LIBDIRS = $(espmock_dir)/libraries/

# ARDUINO_LIB_DIRS = $(espmock_dir)/libraries/

ESPMOCK_INCLUDES=$(foreach esplib,$(ESP_LIBS), -I$(ESPMOCK_LIBDIRS)/${esplib}/src)

CPPFLAGS = $(ESPMOCK_INCLUDES) -I$(espmock_dir)/src -I$(espmock_dir)/src/cores/$(ESP_MODEL)

include ../../../EpoxyDuino/EpoxyDuino.mk

truc:
	@echo "TRAC=$(TRAC)"
	@echo "ESP_LIBS=$(ESP_LIBS)"
	@echo "ESPMOCK_INCLUDES=$(ESPMOCK_INCLUDES)"
	@echo "espmock_dir=$(espmock_dir)"
	@echo "$(ARDUINO_LIB_DIRS)"

