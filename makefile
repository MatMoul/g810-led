CC=g++
CFLAGS=-Wall -O2 -std=gnu++11
LDFLAGS=-lusb-1.0
PROGN=g810-led
SYSTEMDDIR?=/usr/lib/systemd

.PHONY: all debug clean

all: bin/$(PROGN)

bin/$(PROGN): src/main.cpp src/helpers/*.cpp src/helpers/*.h src/classes/*.cpp src/classes/*.h
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	
debug: CFLAGS += -g -Wextra -pedantic
debug: bin/$(PROGN)
	
clean:
	@rm -rf bin
	
install:
	@install -m 755 -d \
		$(DESTDIR)/etc/$(PROGN)/samples \
		$(DESTDIR)/etc/udev/rules.d \
		$(DESTDIR)$(SYSTEMDDIR)/system \
		$(DESTDIR)/usr/bin
	@cp bin/$(PROGN) $(DESTDIR)/usr/bin
	@test -s $(DESTDIR)/usr/bin/g410-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g410-led
	@test -s $(DESTDIR)/usr/bin/g610-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g610-led
	@test -s $(DESTDIR)/usr/bin/g910-led || ln -s /usr/bin/$(PROGN) $(DESTDIR)/usr/bin/g910-led
	@cp udev/$(PROGN).rules $(DESTDIR)/etc/udev/rules.d
	@cp sample_profiles/* $(DESTDIR)/etc/$(PROGN)/samples
	@test -s $(DESTDIR)/etc/$(PROGN)/profile || cp $(DESTDIR)/etc/$(PROGN)/samples/group_keys $(DESTDIR)/etc/$(PROGN)/profile
	@test -s $(DESTDIR)/etc/$(PROGN)/reboot || cp $(DESTDIR)/etc/$(PROGN)/samples/all_off $(DESTDIR)/etc/$(PROGN)/reboot
	@cp systemd/$(PROGN).service $(DESTDIR)$(SYSTEMDDIR)/system
	@cp systemd/$(PROGN)-reboot.service $(DESTDIR)$(SYSTEMDDIR)/system
	@udevadm control --reload-rules
	@systemctl daemon-reload
	@systemctl start $(PROGN)
	@systemctl enable $(PROGN)
	@systemctl enable $(PROGN)-reboot

uninstall:
	@systemctl disable $(PROGN)
	@systemctl disable $(PROGN)-reboot
	@rm $(SYSTEMDDIR)/system/$(PROGN).service
	@rm $(SYSTEMDDIR)/system/$(PROGN)-reboot.service
	@systemctl daemon-reload
	@rm /usr/bin/g410-led
	@rm /usr/bin/g610-led
	@rm /usr/bin/g910-led
	@rm /usr/bin/$(PROGN)
	@rm -R /etc/$(PROGN)
	@rm /etc/udev/rules.d/$(PROGN).rules
	@udevadm control --reload-rules
