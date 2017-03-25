CC=g++
CFLAGS=-Wall -O2 -std=gnu++11
LIB?=hidapi
ifeq ($(LIB),libusb)
	CPPFLAGS=-Dlibusb -lpthread
	LDFLAGS=-lusb-1.0
else
	CPPFLAGS=-Dhidapi -lpthread
	LDFLAGS=-lhidapi-hidraw
endif
PROGN=g810-led
SYSTEMDDIR?=/usr/lib/systemd

.PHONY: all debug clean setup install uninstall

all: bin/$(PROGN)

bin/$(PROGN): src/main.cpp src/helpers/*.cpp src/helpers/*.h src/classes/*.cpp src/classes/*.h
	@mkdir -p bin
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	
debug: CFLAGS += -g -Wextra -pedantic
debug: bin/$(PROGN)
	
clean:
	@rm -rf bin

setup:
	@install -m 755 -d \
		$(DESTDIR)/usr/bin \
		$(DESTDIR)/etc/$(PROGN)/samples \
		$(DESTDIR)/etc/udev/rules.d
	@cp bin/$(PROGN) $(DESTDIR)/usr/bin
	@test -s $(DESTDIR)/usr/bin/g410-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g410-led
	@test -s $(DESTDIR)/usr/bin/g610-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g610-led
	@test -s $(DESTDIR)/usr/bin/g910-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g910-led
	@cp sample_profiles/* $(DESTDIR)/etc/$(PROGN)/samples
	@cp udev/$(PROGN).rules $(DESTDIR)/etc/udev/rules.d
	@test -s /usr/bin/systemd-run && \
		install -m 755 -d $(DESTDIR)$(SYSTEMDDIR)/system && \
		cp systemd/$(PROGN).service $(DESTDIR)$(SYSTEMDDIR)/system && \
		cp systemd/$(PROGN)-reboot.service $(DESTDIR)$(SYSTEMDDIR)/system
	
install: setup
	@test -s /etc/$(PROGN)/profile || \
		cp /etc/$(PROGN)/samples/group_keys /etc/$(PROGN)/profile
	@test -s /etc/$(PROGN)/reboot || \
		cp /etc/$(PROGN)/samples/all_off /etc/$(PROGN)/reboot
	@udevadm control --reload-rules
	@test -s /usr/bin/systemd-run && \
		systemctl daemon-reload && \
		systemctl start $(PROGN) && \
		systemctl enable $(PROGN) && \
		systemctl enable $(PROGN)-reboot
	
uninstall:
	@test -s /usr/bin/systemd-run && \
		systemctl disable $(PROGN) && \
		systemctl disable $(PROGN)-reboot && \
		rm $(SYSTEMDDIR)/system/$(PROGN).service && \
		rm $(SYSTEMDDIR)/system/$(PROGN)-reboot.service && \
		systemctl daemon-reload && \
		rm -R /etc/$(PROGN)
	
	@rm /usr/bin/g410-led
	@rm /usr/bin/g610-led
	@rm /usr/bin/g910-led
	@rm /usr/bin/$(PROGN)
	
	@rm /etc/udev/rules.d/$(PROGN).rules
	@udevadm control --reload-rules
