CC=g++
CFLAGS=-Wall -O2 -std=gnu++11
LIBUSB_INC?=-I/usr/include/libusb-1.0
LDFLAGS=-lusb-1.0
PROGN=g810-led

.PHONY: all debug clean install uninstall update

all: bin/$(PROGN)

bin/$(PROGN): src/main.cpp src/classes/*.cpp src/classes/*.h
	@mkdir -p bin
	$(CC) $(CFLAGS) $(LIBUSB_INC) -o $@ $^ $(LDFLAGS)

debug: CFLAGS += -g -Wextra -pedantic
debug: bin/$(PROGN)

clean:
	@rm -rf bin

install:
	@sudo mkdir -p /etc/$(PROGN)/samples
	@sudo cp sample_profiles/* /etc/$(PROGN)/samples
	@sudo cp udev/$(PROGN).rules /etc/udev/rules.d
	@sudo cp bin/$(PROGN) /usr/bin
	@sudo test -s /etc/$(PROGN)/profile || sudo cp /etc/$(PROGN)/samples/group_keys /etc/$(PROGN)/profile
	@sudo test -s /etc/$(PROGN)/reboot || sudo cp /etc/$(PROGN)/samples/all_off /etc/$(PROGN)/reboot
	@sudo cp systemd/$(PROGN).service /usr/lib/systemd/system
	@sudo cp systemd/$(PROGN)-reboot.service /usr/lib/systemd/system
	@sudo systemctl start $(PROGN)
	@sudo systemctl enable $(PROGN)
	@sudo systemctl enable $(PROGN)-reboot

uninstall:
	@sudo systemctl disable $(PROGN)
	@sudo systemctl disable $(PROGN)-reboot
	@sudo rm /usr/lib/systemd/system/$(PROGN).service
	@sudo rm /usr/lib/systemd/system/$(PROGN)-reboot.service
	@sudo rm /etc/udev/rules.d/$(PROGN).rules
	@sudo rm /usr/bin/$(PROGN)
	@sudo rm -R /etc/$(PROGN)
