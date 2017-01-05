CC=g++
CFLAGS=-Wall -O2 -std=gnu++11
LIBUSB_INC?=-I/usr/include/libusb-1.0
LDFLAGS=-lusb-1.0
PROGN=g810-led

.PHONY: all debug clean install uninstall

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
	@sudo udevadm control --reload-rules
	@sudo cp bin/$(PROGN) /usr/bin
	@sudo test -s /usr/bin/g410-led || sudo ln -s /usr/bin/$(PROGN) /usr/bin/g410-led
	@sudo test -s /usr/bin/g610-led || sudo ln -s /usr/bin/$(PROGN) /usr/bin/g610-led
	@sudo test -s /usr/bin/g910-led || sudo ln -s /usr/bin/$(PROGN) /usr/bin/g910-led
	@sudo test -s /etc/$(PROGN)/profile || sudo cp /etc/$(PROGN)/samples/group_keys /etc/$(PROGN)/profile
	@sudo test -s /etc/$(PROGN)/reboot || sudo cp /etc/$(PROGN)/samples/all_off /etc/$(PROGN)/reboot
	@sudo cp systemd/$(PROGN).service /lib/systemd/system
	@sudo cp systemd/$(PROGN)-reboot.service /lib/systemd/system
	@sudo systemctl daemon-reload
	@sudo systemctl start $(PROGN)
	@sudo systemctl enable $(PROGN)
	@sudo systemctl enable $(PROGN)-reboot

uninstall:
	@sudo systemctl disable $(PROGN)
	@sudo systemctl disable $(PROGN)-reboot
	@sudo rm /lib/systemd/system/$(PROGN).service
	@sudo rm /lib/systemd/system/$(PROGN)-reboot.service
	@sudo systemctl daemon-reload
	@sudo rm /usr/bin/g410-led
	@sudo rm /usr/bin/g610-led
	@sudo rm /usr/bin/g910-led
	@sudo rm /usr/bin/$(PROGN)
	@sudo rm -R /etc/$(PROGN)
	@sudo rm /etc/udev/rules.d/$(PROGN).rules
	@sudo udevadm control --reload-rules
