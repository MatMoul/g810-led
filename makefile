CC=g++
CFLAGS=-Wall -O2 -std=gnu++11
LIBUSB_INC?=-I/usr/include/libusb-1.0
LDFLAGS=-lusb-1.0
PROGN=g810-led

.PHONY: all debug clean

all: bin/$(PROGN)

bin/$(PROGN): src/main.cpp src/classes/*.cpp src/classes/*.h
	@mkdir -p bin
	$(CC) $(CFLAGS) $(LIBUSB_INC) -o $@ $^ $(LDFLAGS)

debug: CFLAGS += -g -Wextra -pedantic
debug: bin/$(PROGN)

clean:
	rm -rf bin

