CC=g++
CFLAGS=
LDFLAGS=-lusb-1.0


all: bin/g810-led

bin/g810-led: src/main.cpp src/classes/*
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)
