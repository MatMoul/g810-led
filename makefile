CC=g++
CFLAGS=-Wall -O2 -std=gnu++11
LIB?=hidapi
ifeq ($(LIB),libusb)
	CPPFLAGS=-Dlibusb
	LDFLAGS=-lusb-1.0
else
	CPPFLAGS=-Dhidapi
	LDFLAGS=-lhidapi-hidraw
endif
SYSTEMDDIR?=/usr/lib/systemd

prefix?=$(DESTDIR)/usr
libdir?=$(prefix)/lib
includedir?=$(prefix)/include

# Program & versioning information
PROGN=g810-led
MAJOR=0
MINOR=2
MICRO=2

CFLAGS+=-DVERSION=\"$(MAJOR).$(MINOR).$(MICRO)\"
APPSRCS=src/main.cpp src/helpers/*.cpp src/helpers/*.h
LIBSRCS=src/classes/*.cpp src/classes/*.h

.PHONY: all bin debug clean setup install uninstall lib install-lib install-dev

all: lib/lib$(PROGN).so bin/$(PROGN)

bin: bin/$(PROGN)

bin/$(PROGN): $(APPSRCS) $(LIBSRCS)
	@mkdir -p bin
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	
debug: CFLAGS += -g -Wextra -pedantic
debug: bin/$(PROGN)

lib/lib$(PROGN).so: $(LIBSRCS)
	@mkdir -p lib
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -shared -Wl,-soname,lib$(PROGN).so -o lib/lib$(PROGN).so.$(MAJOR).$(MINOR).$(MICRO) $^ $(LDFLAGS)
	@ln -sf lib$(PROGN).so.$(MAJOR).$(MINOR).$(MICRO) lib/lib$(PROGN).so

bin-linked: lib/lib$(PROGN).so
	@mkdir -p bin
	$(CC) $(CPPFLAGS) $(CFLAGS) $(APPSRCS) -o bin/$(PROGN) $(LDFLAGS) -L./lib -l$(PROGN)

lib: lib/lib$(PROGN).so

clean:
	@rm -rf bin
	@rm -rf lib

setup:
	@install -m 755 -d \
		$(DESTDIR)/usr/bin \
		$(DESTDIR)/etc/$(PROGN)/samples \
		$(DESTDIR)/etc/udev/rules.d
	@cp bin/$(PROGN) $(DESTDIR)/usr/bin
	@test -s $(DESTDIR)/usr/bin/g213-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g213-led
	@test -s $(DESTDIR)/usr/bin/g410-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g410-led
	@test -s $(DESTDIR)/usr/bin/g610-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g610-led
	@test -s $(DESTDIR)/usr/bin/g910-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g910-led
	@cp sample_profiles/* $(DESTDIR)/etc/$(PROGN)/samples
	@cp udev/$(PROGN).rules $(DESTDIR)/etc/udev/rules.d
	@test -s /usr/bin/systemd-run && \
		install -m 755 -d $(DESTDIR)$(SYSTEMDDIR)/system && \
		cp systemd/$(PROGN)-reboot.service $(DESTDIR)$(SYSTEMDDIR)/system

install-lib: lib
	@install -m 755 -d $(libdir)
	@install -m 644 lib/lib$(PROGN).so.$(MAJOR).$(MINOR).$(MICRO) $(libdir)/
	@ln -sf lib$(PROGN).so.$(MAJOR).$(MINOR).$(MICRO) $(libdir)/lib$(PROGN).so

install-dev: install-lib
	@mkdir -p $(includedir)/$(PROGN)/
	@install -m 644 src/classes/*.h $(includedir)/$(PROGN)

install: setup
	@test -s /etc/$(PROGN)/profile || \
		cp /etc/$(PROGN)/samples/group_keys /etc/$(PROGN)/profile
	@test -s /etc/$(PROGN)/reboot || \
		cp /etc/$(PROGN)/samples/all_off /etc/$(PROGN)/reboot
	@udevadm control --reload-rules
	@$(PROGN) -p /etc/$(PROGN)/profile
	@test -s /usr/bin/systemd-run && \
		systemctl daemon-reload && \
		systemctl enable $(PROGN)-reboot

uninstall-lib:
	@rm -f $(libdir)/lib$(PROGN).so*

uninstall-dev:
	@rm -rf $(includedir)/$(PROGN)

uninstall:
	@test -s /usr/bin/systemd-run && \
		systemctl disable $(PROGN) && \
		systemctl disable $(PROGN)-reboot && \
		rm $(SYSTEMDDIR)/system/$(PROGN).service && \
		rm $(SYSTEMDDIR)/system/$(PROGN)-reboot.service && \
		systemctl daemon-reload && \
		rm -R /etc/$(PROGN)
	
	@rm /usr/bin/g213-led
	@rm /usr/bin/g410-led
	@rm /usr/bin/g610-led
	@rm /usr/bin/g910-led
	@rm /usr/bin/$(PROGN)
	
	@rm /etc/udev/rules.d/$(PROGN).rules
	@udevadm control --reload-rules
