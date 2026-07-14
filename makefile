UNAME_S := $(shell uname -s)

CXX?=g++
CXXFLAGS?=-Wall -O2
LIB?=hidapi
ifeq ($(LIB),libusb)
	ifeq ($(UNAME_S),Darwin)
		CPPFLAGS+=-Dlibusb
		LIBUSB_PREFIX?=$(shell brew --prefix libusb 2>/dev/null)
		ifneq ($(strip $(LIBUSB_PREFIX)),)
			CPPFLAGS+=-I$(LIBUSB_PREFIX)/include
			LDFLAGS+=-L$(LIBUSB_PREFIX)/lib
		endif
		LIBS?=-lusb-1.0
	else
		CPPFLAGS=-Dlibusb
		LIBS=-lusb-1.0
	endif
else
	ifeq ($(UNAME_S),Darwin)
		CPPFLAGS+=-Dhidapi
		HIDAPI_PREFIX?=$(shell brew --prefix hidapi 2>/dev/null)
		ifneq ($(strip $(HIDAPI_PREFIX)),)
			CPPFLAGS+=-I$(HIDAPI_PREFIX)/include
			LDFLAGS+=-L$(HIDAPI_PREFIX)/lib
		endif
		LIBS?=-lhidapi
	else
		CPPFLAGS=-Dhidapi
		LIBS=-lhidapi-hidraw
	endif
endif
SYSTEMDDIR?=/usr/lib/systemd

ifeq ($(UNAME_S),Darwin)
PREFIX?=/usr/local
libdir?=$(DESTDIR)$(PREFIX)/lib
includedir?=$(DESTDIR)$(PREFIX)/include
bindir?=$(DESTDIR)$(PREFIX)/bin
sysconfdir?=$(DESTDIR)$(PREFIX)/etc
else
PREFIX?=$(DESTDIR)/usr
libdir?=$(PREFIX)/lib
includedir?=$(PREFIX)/include
endif

# Program & versioning information
PROGN=g810-led
MAJOR=0
MINOR=4
MICRO=3

CXXFLAGS+=-std=gnu++11 -DVERSION=\"$(MAJOR).$(MINOR).$(MICRO)\"
APPSRCS=src/main.cpp src/helpers/*.cpp
LIBSRCS=src/classes/*.cpp

ifeq ($(UNAME_S),Darwin)
SHLIB_EXT=dylib
LIBRARY=lib/lib$(PROGN).$(MAJOR).$(MINOR).$(MICRO).$(SHLIB_EXT)
SHARED_FLAGS=-dynamiclib -Wl,-install_name,@rpath/lib$(PROGN).$(MAJOR).dylib \
	-Wl,-compatibility_version,$(MAJOR) -Wl,-current_version,$(MAJOR).$(MINOR).$(MICRO)
BIN_LINKED_RPATH=-Wl,-rpath,@loader_path/../lib
endif

.PHONY: all bin debug clean setup install uninstall lib install-lib install-dev

ifeq ($(UNAME_S),Darwin)
all: $(LIBRARY) bin/$(PROGN)
else
all: lib/lib$(PROGN).so bin/$(PROGN)
endif

bin: bin/$(PROGN)

bin/$(PROGN): $(APPSRCS) $(LIBSRCS)
	@mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)
	
debug: CXXFLAGS += -g -Wextra -pedantic
debug: bin/$(PROGN)

ifeq ($(UNAME_S),Darwin)
$(LIBRARY): $(LIBSRCS)
	@mkdir -p lib
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(SHARED_FLAGS) -o $@ $^ $(LIBS)
	@ln -sf $(notdir $@) lib/lib$(PROGN).$(SHLIB_EXT)
	@ln -sf $(notdir $@) lib/lib$(PROGN).$(MAJOR).$(SHLIB_EXT)
else
lib/lib$(PROGN).so: $(LIBSRCS)
	@mkdir -p lib
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -fPIC -shared -Wl,-soname,lib$(PROGN).so -o lib/lib$(PROGN).so.$(MAJOR).$(MINOR).$(MICRO) $^ $(LIBS)
	@ln -sf lib$(PROGN).so.$(MAJOR).$(MINOR).$(MICRO) lib/lib$(PROGN).so
endif

ifeq ($(UNAME_S),Darwin)
bin-linked: $(LIBRARY)
	@mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(APPSRCS) -o bin/$(PROGN) $(LIBS) -L./lib -l$(PROGN) $(BIN_LINKED_RPATH)
else
bin-linked: lib/lib$(PROGN).so
	@mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(APPSRCS) -o bin/$(PROGN) $(LIBS) -L./lib -l$(PROGN)
endif

ifeq ($(UNAME_S),Darwin)
lib: $(LIBRARY)
else
lib: lib/lib$(PROGN).so
endif

clean:
	@rm -rf bin
	@rm -rf lib

ifeq ($(UNAME_S),Darwin)
setup:
	@install -m 755 -d $(bindir) $(sysconfdir)/$(PROGN)/samples
	@install -m 755 bin/$(PROGN) $(bindir)/
	@for alias in g213 g410 g413 g512 g513 g610 g815 g910 gpro; do \
		ln -sf $(PROGN) $(bindir)/$$alias-led; \
	done
	@cp sample_profiles/* $(sysconfdir)/$(PROGN)/samples
else
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
	@test -s $(DESTDIR)/usr/bin/g815-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g815-led
	@test -s $(DESTDIR)/usr/bin/g910-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g910-led
	@test -s $(DESTDIR)/usr/bin/gpro-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/gpro-led
	@cp sample_profiles/* $(DESTDIR)/etc/$(PROGN)/samples
	@cp udev/$(PROGN).rules $(DESTDIR)/etc/udev/rules.d
	@test -s /usr/bin/systemd-run && \
		install -m 755 -d $(DESTDIR)$(SYSTEMDDIR)/system && \
		cp systemd/$(PROGN)-reboot.service $(DESTDIR)$(SYSTEMDDIR)/system
endif

install-lib: lib
	@install -m 755 -d $(libdir)
ifeq ($(UNAME_S),Darwin)
	@install -m 755 $(LIBRARY) $(libdir)/
	@ln -sf $(notdir $(LIBRARY)) $(libdir)/lib$(PROGN).$(SHLIB_EXT)
	@ln -sf $(notdir $(LIBRARY)) $(libdir)/lib$(PROGN).$(MAJOR).$(SHLIB_EXT)
else
	@install -m 644 lib/lib$(PROGN).so.$(MAJOR).$(MINOR).$(MICRO) $(libdir)/
	@ln -sf lib$(PROGN).so.$(MAJOR).$(MINOR).$(MICRO) $(libdir)/lib$(PROGN).so
endif

install-dev: install-lib
	@mkdir -p $(includedir)/$(PROGN)/
	@install -m 644 src/classes/*.h $(includedir)/$(PROGN)

ifeq ($(UNAME_S),Darwin)
install: setup
	@test -s $(sysconfdir)/$(PROGN)/profile || \
		cp $(sysconfdir)/$(PROGN)/samples/group_keys $(sysconfdir)/$(PROGN)/profile
	@test -s $(sysconfdir)/$(PROGN)/reboot || \
		cp $(sysconfdir)/$(PROGN)/samples/all_off $(sysconfdir)/$(PROGN)/reboot
else
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
endif

uninstall-lib:
ifeq ($(UNAME_S),Darwin)
	@rm -f $(libdir)/lib$(PROGN)*.$(SHLIB_EXT)
else
	@rm -f $(libdir)/lib$(PROGN).so*
endif

uninstall-dev:
	@rm -rf $(includedir)/$(PROGN)

ifeq ($(UNAME_S),Darwin)
uninstall:
	@rm -f $(bindir)/g213-led $(bindir)/g410-led $(bindir)/g413-led \
		$(bindir)/g512-led $(bindir)/g513-led $(bindir)/g610-led \
		$(bindir)/g815-led $(bindir)/g910-led $(bindir)/gpro-led $(bindir)/$(PROGN)
else
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
	@rm /usr/bin/g815-led
	@rm /usr/bin/g910-led
	@rm /usr/bin/gpro-led
	@rm /usr/bin/$(PROGN)
	
	@rm /etc/udev/rules.d/$(PROGN).rules
	@udevadm control --reload-rules
endif
