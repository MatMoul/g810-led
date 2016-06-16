CC=g++
CFLAGS=-Wall -O2 -std=gnu++11
LDFLAGS=-lusb-1.0
PROGN=g810-led

.PHONY: all clean

all: bin/$(PROGN)


bin/$(PROGN): src/main.cpp src/classes/*.cpp
	@mkdir -p bin
	$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

clean:
	rm -rf bin

