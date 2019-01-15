CXX?=g++
CXXFLAGS?=-Wall -O2
LIB?=hidapi
ifeq ($(LIB),libusb)
	CPPFLAGS=-Dlibusb
	LIBS=-lusb-1.0
else
	CPPFLAGS=-Dhidapi
	LIBS=-lhidapi-hidraw
endif
SYSTEMDDIR?=/usr/lib/systemd

PREFIX?=$(DESTDIR)/usr
libdir?=$(PREFIX)/lib
includedir?=$(PREFIX)/include

# Program & versioning information
PROGN=g810-led
MAJOR=0
MINOR=3
MICRO=2

CXXFLAGS+=-std=gnu++11 -DVERSION=\"$(MAJOR).$(MINOR).$(MICRO)\"
APPSRCS=src/main.cpp src/helpers/*.cpp src/helpers/*.h
LIBSRCS=src/classes/*.cpp src/classes/*.h

.PHONY: all bin debug clean setup install uninstall lib install-lib install-dev

all: lib/lib$(PROGN).so bin/$(PROGN)

bin: bin/$(PROGN)

bin/$(PROGN): $(APPSRCS) $(LIBSRCS)
	@mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)
	
debug: CXXFLAGS += -g -Wextra -pedantic
debug: bin/$(PROGN)

lib/lib$(PROGN).so: $(LIBSRCS)
	@mkdir -p lib
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -fPIC -shared -Wl,-soname,lib$(PROGN).so -o lib/lib$(PROGN).so.$(MAJOR).$(MINOR).$(MICRO) $^ $(LIBS)
	@ln -sf lib$(PROGN).so.$(MAJOR).$(MINOR).$(MICRO) lib/lib$(PROGN).so

bin-linked: lib/lib$(PROGN).so
	@mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(APPSRCS) -o bin/$(PROGN) $(LIBS) -L./lib -l$(PROGN)

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
	@test -s $(DESTDIR)/usr/bin/g413-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g413-led
	@test -s $(DESTDIR)/usr/bin/g512-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g512-led
	@test -s $(DESTDIR)/usr/bin/g513-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g513-led
	@test -s $(DESTDIR)/usr/bin/g610-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g610-led
	@test -s $(DESTDIR)/usr/bin/g910-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g910-led
	@test -s $(DESTDIR)/usr/bin/gpro-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/gpro-led
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
		systemctl disable $(PROGN)-reboot && \
		rm $(SYSTEMDDIR)/system/$(PROGN)-reboot.service && \
		systemctl daemon-reload && \
		rm -R /etc/$(PROGN)
	
	@rm /usr/bin/g213-led
	@rm /usr/bin/g410-led
	@rm /usr/bin/g413-led
	@rm /usr/bin/g512-led
	@rm /usr/bin/g513-led
	@rm /usr/bin/g610-led
	@rm /usr/bin/g910-led
	@rm /usr/bin/gpro-led
	@rm /usr/bin/$(PROGN)
	
	@rm /etc/udev/rules.d/$(PROGN).rules
	@udevadm control --reload-rules
