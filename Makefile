# mingw-w64 Makefile
#
UNAME_S := $(shell uname -s)

# CFLAGS -g -O0 for debug symbols
CC = x86_64-w64-mingw32-gcc-win32
CFLAGS ?= -s -Os
CFLAGS += -Wall -Wextra -std=c99 -pedantic
CFLAGS += -DPROGRAM_VERSION=\"0.1\" -I.


all: tests

# run make on directory
tests:
	(cd TESTS && $(MAKE))
