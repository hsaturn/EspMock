ifeq ($(compiler), gcc)
  CXX=g++
  EXTRA_CXXFLAGS+=-std=c++17
else
  CXX=clang++
  ifeq ($(sanitize), 1)
    EXTRA_CXXFLAGS+=-fsanitize=memory -fsanitize-recover=memory
  endif
	EXTRA_CXXFLAGS+=-stdlib=libc++
endif
EXTRA_CXXFLAGS+=-g3 -O0

.SILENT:

include ../../../EpoxyDuino/EpoxyDuino.mk
