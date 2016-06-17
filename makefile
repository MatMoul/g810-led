CC=g++
# CFLAGS=-Wall -O2 -std=gnu++11 (Problem with this line, g810-led -a ff0000, missing set key)
CFLAGS=-Wall -std=gnu++11
LDFLAGS=-lusb-1.0
PROGN=g810-led

.PHONY: all debug clean

all: bin/$(PROGN)

bin/$(PROGN): src/main.cpp src/classes/*.cpp
	@mkdir -p bin
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

debug: CFLAGS += -g
debug: bin/$(PROGN)

clean:
	rm -rf bin

