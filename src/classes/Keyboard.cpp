#include "Keyboard.h"
#include <vector>
#include <unistd.h>
#include <algorithm>
#include "libusb.h"


bool Keyboard::isAttached() {
	return m_isAttached;
}

bool Keyboard::attach() {
	if (m_isAttached == true) return false;
	int r;
	r = libusb_init(&ctx);
	if (r < 0) return false;
	
	libusb_device **devs;
	ssize_t cnt = libusb_get_device_list(ctx, &devs);
	if(cnt < 0) return false;
	int pid = 0;
	for(ssize_t i = 0; i < cnt; i++) {
		libusb_device *device = devs[i];
		libusb_device_descriptor desc = {
			0, // bLength
			0, // bDescriptorType
			0, // bcdUSB
			0, // bDeviceClass
			0, // bDeviceSubClass
			0, // bDeviceProtocol
			0, // bMaxPacketSize0
			0, // idVendor
			0, // idProduct
			0, // bcdDevice
			0, // iManufacturer
			0, // iProduct
			0, // iSerialNumber
			0  // bNumConfigurations
		};
		libusb_get_device_descriptor(device, &desc);
		if (desc.idVendor == 0x046d) {
			if (desc.idProduct == 0xc331) { pid = desc.idProduct; break; } // G810 spectrum
			if (desc.idProduct == 0xc337) { pid = desc.idProduct; break; } // G810 spectrum
			if (desc.idProduct == 0xc330) { pid = desc.idProduct; break; } // G410 spectrum
			if (desc.idProduct == 0xc333) { pid = desc.idProduct; break; } // G610 spectrum
			if (desc.idProduct == 0xc32b) { // G910 spark
				pid = desc.idProduct;
				kbdProtocol = KeyboardProtocol::g910;
				break;
			}
			if (desc.idProduct == 0xc335) { // G910 spectrum
				pid = desc.idProduct;
				kbdProtocol = KeyboardProtocol::g910;
				break;
			}
		}
	}
	libusb_free_device_list(devs, 1);
	if (pid == 0) {
		libusb_exit(ctx);
		ctx = NULL;
		return false;
	}
	
	dev_handle = libusb_open_device_with_vid_pid(ctx, 0x046d, pid);
	if (dev_handle == NULL) {
		libusb_exit(ctx);
		ctx = NULL;
		return false;
	}
	if(libusb_kernel_driver_active(dev_handle, 1) == 1) {
		libusb_detach_kernel_driver(dev_handle, 1);
		m_isKernellDetached = true;
	}
	r = libusb_claim_interface(dev_handle, 1);
	if(r < 0) return false;
	m_isAttached = true;
	return true;
}

bool Keyboard::detach() {
	if (m_isAttached == false) return false;
	int r;
	r = libusb_release_interface(dev_handle, 1);
	if(r!=0) return false;
	if(m_isKernellDetached==true) {
		libusb_attach_kernel_driver(dev_handle, 1);
		m_isKernellDetached = false;
	}
	libusb_close(dev_handle);
	dev_handle = NULL;
	libusb_exit(ctx);
	ctx = NULL;
	m_isAttached = false;
	return true;
}

bool Keyboard::commit() {
	if (m_isAttached == false) return false;
	bool retval = false;
	unsigned char *data = new unsigned char[20];
	switch (kbdProtocol) {
		case KeyboardProtocol::generic:
			data[0] = 0x11;
			data[1] = 0xff;
			data[2] = 0x0c;
			data[3] = 0x5a;
			break;
		case KeyboardProtocol::g910:
			data[0] = 0x11;
			data[1] = 0xff;
			data[2] = 0x0f;
			data[3] = 0x5d;
			break;
		default:
			return false;
			break;
	}
	for(int i = 4; i < 20; i++) data[i] = 0x00;
	retval = sendDataInternal(data, 20);
	delete[] data;
	return retval;
}

bool Keyboard::getKeyAddress(Key key, KeyAddress &keyAddress) {
	switch (key) {
		case Key::logo:
			keyAddress.addressGroup = KeyAddressGroup::logo;
			keyAddress.id = 0x01;
			break;
		case Key::logo2:
			keyAddress.addressGroup = KeyAddressGroup::logo;
			keyAddress.id = 0x02;
			break;
		case Key::backlight:
			keyAddress.addressGroup = KeyAddressGroup::indicators;
			keyAddress.id = 0x01;
			break;
		case Key::game:
			keyAddress.addressGroup = KeyAddressGroup::indicators;
			keyAddress.id = 0x02;
			break;
		case Key::caps:
			keyAddress.addressGroup = KeyAddressGroup::indicators;
			keyAddress.id = 0x03;
			break;
		case Key::scroll:
			keyAddress.addressGroup = KeyAddressGroup::indicators;
			keyAddress.id = 0x04;
			break;
		case Key::num:
			keyAddress.addressGroup = KeyAddressGroup::indicators;
			keyAddress.id = 0x05;
			break;
		case Key::next:
			keyAddress.addressGroup = KeyAddressGroup::multimedia;
			keyAddress.id = 0xb5;
			break;
		case Key::prev:
			keyAddress.addressGroup = KeyAddressGroup::multimedia;
			keyAddress.id = 0xb6;
			break;
		case Key::stop:
			keyAddress.addressGroup = KeyAddressGroup::multimedia;
			keyAddress.id = 0xb7;
			break;
		case Key::play:
			keyAddress.addressGroup = KeyAddressGroup::multimedia;
			keyAddress.id = 0xcd;
			break;
		case Key::mute:
			keyAddress.addressGroup = KeyAddressGroup::multimedia;
			keyAddress.id = 0xe2;
			break;
		case Key::g1:
			keyAddress.addressGroup = KeyAddressGroup::gkeys;
			keyAddress.id = 0x01;
			break;
		case Key::g2:
			keyAddress.addressGroup = KeyAddressGroup::gkeys;
			keyAddress.id = 0x02;
			break;
		case Key::g3:
			keyAddress.addressGroup = KeyAddressGroup::gkeys;
			keyAddress.id = 0x03;
			break;
		case Key::g4:
			keyAddress.addressGroup = KeyAddressGroup::gkeys;
			keyAddress.id = 0x04;
			break;
		case Key::g5:
			keyAddress.addressGroup = KeyAddressGroup::gkeys;
			keyAddress.id = 0x05;
			break;
		case Key::g6:
			keyAddress.addressGroup = KeyAddressGroup::gkeys;
			keyAddress.id = 0x06;
			break;
		case Key::g7:
			keyAddress.addressGroup = KeyAddressGroup::gkeys;
			keyAddress.id = 0x07;
			break;
		case Key::g8:
			keyAddress.addressGroup = KeyAddressGroup::gkeys;
			keyAddress.id = 0x08;
			break;
		case Key::g9:
			keyAddress.addressGroup = KeyAddressGroup::gkeys;
			keyAddress.id = 0x09;
			break;
		default:
			keyAddress.addressGroup = KeyAddressGroup::keys;
			switch (key) {
				case Key::a: keyAddress.id = 0x04; break;
				case Key::b: keyAddress.id = 0x05; break;
				case Key::c: keyAddress.id = 0x06; break;
				case Key::d: keyAddress.id = 0x07; break;
				case Key::e: keyAddress.id = 0x08; break;
				case Key::f: keyAddress.id = 0x09; break;
				case Key::g: keyAddress.id = 0x0a; break;
				case Key::h: keyAddress.id = 0x0b; break;
				case Key::i: keyAddress.id = 0x0c; break;
				case Key::j: keyAddress.id = 0x0d; break;
				case Key::k: keyAddress.id = 0x0e; break;
				case Key::l: keyAddress.id = 0x0f; break;
				case Key::m: keyAddress.id = 0x10; break;
				case Key::n: keyAddress.id = 0x11; break;
				case Key::o: keyAddress.id = 0x12; break;
				case Key::p: keyAddress.id = 0x13; break;
				case Key::q: keyAddress.id = 0x14; break;
				case Key::r: keyAddress.id = 0x15; break;
				case Key::s: keyAddress.id = 0x16; break;
				case Key::t: keyAddress.id = 0x17; break;
				case Key::u: keyAddress.id = 0x18; break;
				case Key::v: keyAddress.id = 0x19; break;
				case Key::w: keyAddress.id = 0x1a; break;
				case Key::x: keyAddress.id = 0x1b; break;
				case Key::z: keyAddress.id = 0x1c; break;
				case Key::y: keyAddress.id = 0x1d; break;
				case Key::n1: keyAddress.id = 0x1e; break;
				case Key::n2: keyAddress.id = 0x1f; break;
				case Key::n3: keyAddress.id = 0x20; break;
				case Key::n4: keyAddress.id = 0x21; break;
				case Key::n5: keyAddress.id = 0x22; break;
				case Key::n6: keyAddress.id = 0x23; break;
				case Key::n7: keyAddress.id = 0x24; break;
				case Key::n8: keyAddress.id = 0x25; break;
				case Key::n9: keyAddress.id = 0x26; break;
				case Key::n0: keyAddress.id = 0x27; break;
				case Key::enter: keyAddress.id = 0x28; break;
				case Key::esc: keyAddress.id = 0x29; break;
				case Key::backspace: keyAddress.id = 0x2a; break;
				case Key::tab: keyAddress.id = 0x2b; break;
				case Key::space: keyAddress.id = 0x2c; break;
				case Key::minus: keyAddress.id = 0x2d; break;
				case Key::equal: keyAddress.id = 0x2e; break;
				case Key::open_bracket: keyAddress.id = 0x2f; break;
				case Key::close_bracket: keyAddress.id = 0x30; break;
				case Key::backslash: keyAddress.id = 0x31; break;
				case Key::dollar: keyAddress.id = 0x32; break;
				case Key::semicolon: keyAddress.id = 0x33; break;
				case Key::quote: keyAddress.id = 0x34; break;
				case Key::tilde: keyAddress.id = 0x35; break;
				case Key::comma: keyAddress.id = 0x36; break;
				case Key::period: keyAddress.id = 0x37; break;
				case Key::slash: keyAddress.id = 0x38; break;
				case Key::caps_lock: keyAddress.id = 0x39; break;
				case Key::f1: keyAddress.id = 0x3a; break;
				case Key::f2: keyAddress.id = 0x3b; break;
				case Key::f3: keyAddress.id = 0x3c; break;
				case Key::f4: keyAddress.id = 0x3d; break;
				case Key::f5: keyAddress.id = 0x3e; break;
				case Key::f6: keyAddress.id = 0x3f; break;
				case Key::f7: keyAddress.id = 0x40; break;
				case Key::f8: keyAddress.id = 0x41; break;
				case Key::f9: keyAddress.id = 0x42; break;
				case Key::f10: keyAddress.id = 0x43; break;
				case Key::f11: keyAddress.id = 0x44; break;
				case Key::f12: keyAddress.id = 0x45; break;
				case Key::print_screen: keyAddress.id = 0x46; break;
				case Key::scroll_lock: keyAddress.id = 0x47; break;
				case Key::pause_break: keyAddress.id = 0x48; break;
				case Key::insert: keyAddress.id = 0x49; break;
				case Key::home: keyAddress.id = 0x4a; break;
				case Key::page_up: keyAddress.id = 0x4b; break;
				case Key::del: keyAddress.id = 0x4c; break;
				case Key::end: keyAddress.id = 0x4d; break;
				case Key::page_down: keyAddress.id = 0x4e; break;
				case Key::arrow_right: keyAddress.id = 0x4f; break;
				case Key::arrow_left: keyAddress.id = 0x50; break;
				case Key::arrow_bottom: keyAddress.id = 0x51; break;
				case Key::arrow_top: keyAddress.id = 0x52; break;
				case Key::num_lock: keyAddress.id = 0x53; break;
				case Key::num_slash: keyAddress.id = 0x54; break;
				case Key::num_asterisk: keyAddress.id = 0x55; break;
				case Key::num_minus: keyAddress.id = 0x56; break;
				case Key::num_plus: keyAddress.id = 0x57; break;
				case Key::num_enter: keyAddress.id = 0x58; break;
				case Key::num_1: keyAddress.id = 0x59; break;
				case Key::num_2: keyAddress.id = 0x5a; break;
				case Key::num_3: keyAddress.id = 0x5b; break;
				case Key::num_4: keyAddress.id = 0x5c; break;
				case Key::num_5: keyAddress.id = 0x5d; break;
				case Key::num_6: keyAddress.id = 0x5e; break;
				case Key::num_7: keyAddress.id = 0x5f; break;
				case Key::num_8: keyAddress.id = 0x60; break;
				case Key::num_9: keyAddress.id = 0x61; break;
				case Key::num_0: keyAddress.id = 0x62; break;
				case Key::num_dot: keyAddress.id = 0x63; break;
				case Key::intl_backslash: keyAddress.id = 0x64; break;
				case Key::menu: keyAddress.id = 0x65; break;
				case Key::ctrl_left: keyAddress.id = 0xe0; break;
				case Key::shift_left: keyAddress.id = 0xe1; break;
				case Key::alt_left: keyAddress.id = 0xe2; break;
				case Key::win_left: keyAddress.id = 0xe3; break;
				case Key::ctrl_right: keyAddress.id = 0xe4; break;
				case Key::shift_right: keyAddress.id = 0xe5; break;
				case Key::alt_right: keyAddress.id = 0xe6; break;
				case Key::win_right: keyAddress.id = 0xe7; break;
				default: break;
			}
			break;
	}
	return true;
}

bool Keyboard::parsePowerOnEffect(std::string effect, PowerOnEffect &powerOnEffect) {
	if (effect == "rainbow") powerOnEffect = PowerOnEffect::rainbow;
	else if (effect == "color") powerOnEffect = PowerOnEffect::color;
	else return false;
	return true;
}

bool Keyboard::parseKey(std::string key, KeyAddress &keyAddress) {
	std::transform(key.begin(), key.end(), key.begin(), ::tolower);
	Key parsedKey;
	if (key == "logo") parsedKey = Key::logo;
	else if (key == "logo2") parsedKey = Key::logo2;
	else if (key == "back_light" || key == "backlight" || key == "light") parsedKey = Key::backlight;
	else if (key == "game_mode" || key == "gamemode" || key == "game") parsedKey = Key::game;
	else if (key == "caps_indicator" || key == "capsindicator" || key == "caps") parsedKey = Key::caps;
	else if (key == "scroll_indicator" || key == "scrollindicator" || key == "scroll") parsedKey = Key::scroll;
	else if (key == "num_indicator" || key == "numindicator" || key == "num") parsedKey = Key::num;
	else if (key == "next") parsedKey = Key::next;
	else if (key == "prev" || key == "previous") parsedKey = Key::prev;
	else if (key == "stop") parsedKey = Key::stop;
	else if (key == "play_pause" || key == "playpause" || key == "play") parsedKey = Key::play;
	else if (key == "mute") parsedKey = Key::mute;
	else if (key == "a") parsedKey = Key::a;
	else if (key == "b") parsedKey = Key::b;
	else if (key == "c") parsedKey = Key::c;
	else if (key == "d") parsedKey = Key::d;
	else if (key == "e") parsedKey = Key::e;
	else if (key == "f") parsedKey = Key::f;
	else if (key == "g") parsedKey = Key::g;
	else if (key == "h") parsedKey = Key::h;
	else if (key == "i") parsedKey = Key::i;
	else if (key == "j") parsedKey = Key::j;
	else if (key == "k") parsedKey = Key::k;
	else if (key == "l") parsedKey = Key::l;
	else if (key == "m") parsedKey = Key::m;
	else if (key == "n") parsedKey = Key::n;
	else if (key == "o") parsedKey = Key::o;
	else if (key == "p") parsedKey = Key::p;
	else if (key == "q") parsedKey = Key::q;
	else if (key == "r") parsedKey = Key::r;
	else if (key == "s") parsedKey = Key::s;
	else if (key == "t") parsedKey = Key::t;
	else if (key == "u") parsedKey = Key::u;
	else if (key == "v") parsedKey = Key::v;
	else if (key == "w") parsedKey = Key::w;
	else if (key == "x") parsedKey = Key::x;
	else if (key == "z") parsedKey = Key::z;
	else if (key == "y") parsedKey = Key::y;
	else if (key == "1" || key == "one") parsedKey = Key::n1;
	else if (key == "2" || key == "two") parsedKey = Key::n2;
	else if (key == "3" || key == "three") parsedKey = Key::n3;
	else if (key == "4" || key == "four") parsedKey = Key::n4;
	else if (key == "5" || key == "five") parsedKey = Key::n5;
	else if (key == "6" || key == "six") parsedKey = Key::n6;
	else if (key == "7" || key == "seven") parsedKey = Key::n7;
	else if (key == "8" || key == "eight") parsedKey = Key::n8;
	else if (key == "9" || key == "nine") parsedKey = Key::n9;
	else if (key == "0" || key == "zero") parsedKey = Key::n0;
	else if (key == "enter") parsedKey = Key::enter;
	else if (key == "esc" || key == "escape") parsedKey = Key::esc;
	else if (key == "back" || key == "backspace") parsedKey = Key::backspace;
	else if (key == "tab") parsedKey = Key::tab;
	else if (key == "space") parsedKey = Key::space;
	else if (key == "tilde" || key == "~") parsedKey = Key::tilde;
	else if (key == "minus" || key == "-") parsedKey = Key::minus;
	else if (key == "equal" || key == "=") parsedKey = Key::equal;
	else if (key == "open_bracket" || key == "[") parsedKey = Key::open_bracket;
	else if (key == "close_bracket" || key == "]") parsedKey = Key::close_bracket;
	else if (key == "backslash" || key == "\\") parsedKey = Key::backslash;
	else if (key == "semicolon" || key == ";") parsedKey = Key::semicolon;
	else if (key == "quote" || key == "\"") parsedKey = Key::quote;
	else if (key == "dollar" || key == "$") parsedKey = Key::dollar;
	else if (key == "comma" || key == ",") parsedKey = Key::comma;
	else if (key == "period" || key == ".") parsedKey = Key::period;
	else if (key == "slash" || key == "/") parsedKey = Key::slash;
	else if (key == "caps_lock" || key == "capslock") parsedKey = Key::caps_lock;
	else if (key == "f1") parsedKey = Key::f1;
	else if (key == "f2") parsedKey = Key::f2;
	else if (key == "f3") parsedKey = Key::f3;
	else if (key == "f4") parsedKey = Key::f4;
	else if (key == "f5") parsedKey = Key::f5;
	else if (key == "f6") parsedKey = Key::f6;
	else if (key == "f7") parsedKey = Key::f7;
	else if (key == "f8") parsedKey = Key::f8;
	else if (key == "f9") parsedKey = Key::f9;
	else if (key == "f10") parsedKey = Key::f10;
	else if (key == "f11") parsedKey = Key::f11;
	else if (key == "f12") parsedKey = Key::f12;
	else if (key == "print_screen" || key == "printscreen" || key == "printscr" || key == "print") parsedKey = Key::print_screen;
	else if (key == "scroll_lock" || key == "scrolllock") parsedKey = Key::scroll_lock;
	else if (key == "pause_break" || key == "pausebreak" || key == "pause" || key == "break") parsedKey = Key::pause_break;
	else if (key == "insert" || key == "ins") parsedKey = Key::insert;
	else if (key == "home") parsedKey = Key::home;
	else if (key == "page_up" || key == "pageup") parsedKey = Key::page_up;
	else if (key == "delete" || key == "del") parsedKey = Key::del;
	else if (key == "end") parsedKey = Key::end;
	else if (key == "page_down" || key == "pagedown") parsedKey = Key::page_down;
	else if (key == "arrow_right" || key == "arrowright" || key == "right") parsedKey = Key::arrow_right;
	else if (key == "arrow_left" || key == "arrowleft" || key == "left") parsedKey = Key::arrow_left;
	else if (key == "arrow_bottom" || key == "arrowbottom" || key == "bottom") parsedKey = Key::arrow_bottom;
	else if (key == "arrow_top" || key == "arrowtop" || key == "top") parsedKey = Key::arrow_top;
	else if (key == "num_lock" || key == "numlock") parsedKey = Key::num_lock;
	else if (key == "num/" || key == "num_slash" || key == "numslash") parsedKey = Key::num_slash;
	else if (key == "num*" || key == "num_asterisk" || key == "numasterisk") parsedKey = Key::num_asterisk;
	else if (key == "num-" || key == "num_minus" || key == "numminus") parsedKey = Key::num_minus;
	else if (key == "num+" || key == "num_plus" || key == "numplus") parsedKey = Key::num_plus;
	else if (key == "numenter") parsedKey = Key::num_enter;
	else if (key == "num1") parsedKey = Key::num_1;
	else if (key == "num2") parsedKey = Key::num_2;
	else if (key == "num3") parsedKey = Key::num_3;
	else if (key == "num4") parsedKey = Key::num_4;
	else if (key == "num5") parsedKey = Key::num_5;
	else if (key == "num6") parsedKey = Key::num_6;
	else if (key == "num7") parsedKey = Key::num_7;
	else if (key == "num8") parsedKey = Key::num_8;
	else if (key == "num9") parsedKey = Key::num_9;
	else if (key == "num0") parsedKey = Key::num_0;
	else if (key == "num." || key == "num_period" || key == "numperiod") parsedKey = Key::num_dot;
	else if (key == "intl_backslash" || key == "<") parsedKey = Key::intl_backslash;
	else if (key == "menu") parsedKey = Key::menu;
	else if (key == "ctrl_left" || key == "ctrlleft" || key == "ctrll") parsedKey = Key::ctrl_left;
	else if (key == "shift_left" || key == "shiftleft" || key == "shiftl") parsedKey = Key::shift_left;
	else if (key == "alt_left" || key == "altleft" || key == "altl") parsedKey = Key::alt_left;
	else if (key == "win_left" || key == "winleft" || key == "winl") parsedKey = Key::win_left;
	else if (key == "meta_left" || key == "metaleft" || key == "metal") parsedKey = Key::win_left;
	else if (key == "ctrl_right" || key == "ctrlright" || key == "ctrlr") parsedKey = Key::ctrl_right;
	else if (key == "shift_right" || key == "shiftright" || key == "shiftr") parsedKey = Key::shift_right;
	else if (key == "alt_right" || key == "altright" || key == "altr" || key == "altgr") parsedKey = Key::alt_right;
	else if (key == "win_right" || key == "winright" || key == "winr") parsedKey = Key::win_right;
	else if (key == "meta_right" || key == "metaright" || key == "metar") parsedKey = Key::win_right;
	else if (key == "g1") parsedKey = Key::g1;
	else if (key == "g2") parsedKey = Key::g2;
	else if (key == "g3") parsedKey = Key::g3;
	else if (key == "g4") parsedKey = Key::g4;
	else if (key == "g5") parsedKey = Key::g5;
	else if (key == "g6") parsedKey = Key::g6;
	else if (key == "g7") parsedKey = Key::g7;
	else if (key == "g8") parsedKey = Key::g8;
	else if (key == "g9") parsedKey = Key::g9;
	else return false;
	return getKeyAddress(parsedKey, keyAddress);
}

bool Keyboard::parseKeyGroup(std::string key, KeyGroup &keyGroup) {
	if (key == "logo") keyGroup = KeyGroup::logo;
	else if (key == "indicators") keyGroup = KeyGroup::indicators;
	else if (key == "multimedia") keyGroup = KeyGroup::multimedia;
	else if (key == "fkeys") keyGroup = KeyGroup::fkeys;
	else if (key == "modifiers") keyGroup = KeyGroup::modifiers;
	else if (key == "arrows") keyGroup = KeyGroup::arrows;
	else if (key == "numeric") keyGroup = KeyGroup::numeric;
	else if (key == "functions") keyGroup = KeyGroup::functions;
	else if (key == "keys") keyGroup = KeyGroup::keys;
	else if (key == "gkeys") keyGroup = KeyGroup::gkeys;
	else return false;
	return true;
}

bool Keyboard::parseColor(std::string color, KeyColors &colors) {
	if (color.length() == 2) color = color + "0000";  // For G610
	if (color.length() != 6) return false;
	colors.red = std::stoul("0x"+color.substr(0,2), nullptr, 16);
	colors.green = std::stoul("0x"+color.substr(2,2), nullptr, 16);
	colors.blue = std::stoul("0x"+color.substr(4,2), nullptr, 16);
	return true;
}

bool Keyboard::sendDataInternal(unsigned char *data, uint16_t data_size) {
	if (m_isAttached == false) return false;
	int r;
	if (data_size > 20) r = libusb_control_transfer(dev_handle, 0x21, 0x09, 0x0212, 1, data, data_size, 2000);
	else r = libusb_control_transfer(dev_handle, 0x21, 0x09, 0x0211, 1, data, data_size, 2000);
	usleep(1000);
	if (r < 0) return false;
	return true;
}

bool Keyboard::populateAddressGroupInternal(KeyAddressGroup addressGroup, unsigned char *data) {
	switch (kbdProtocol) {
		case KeyboardProtocol::generic:
			switch (addressGroup) {
				case KeyAddressGroup::logo:
					data[0] = 0x11;  // Base address
					data[1] = 0xff;  // Base address
					data[2] = 0x0c;  // Base address
					data[3] = 0x3a;  // Base address
					data[4] = 0x00;  // Base address
					data[5] = 0x10;  // Base address
					data[6] = 0x00;  // Base address
					data[7] = 0x01;  // Base address
					break;
				case KeyAddressGroup::indicators:
					data[0] = 0x12;  // Base address
					data[1] = 0xff;  // Base address
					data[2] = 0x0c;  // Base address
					data[3] = 0x3a;  // Base address
					data[4] = 0x00;  // Base address
					data[5] = 0x40;  // Base address
					data[6] = 0x00;  // Base address
					data[7] = 0x05;  // Base address
					break;
				case KeyAddressGroup::multimedia:
					data[0] = 0x12;  // Base address
					data[1] = 0xff;  // Base address
					data[2] = 0x0c;  // Base address
					data[3] = 0x3a;  // Base address
					data[4] = 0x00;  // Base address
					data[5] = 0x02;  // Base address
					data[6] = 0x00;  // Base address
					data[7] = 0x05;  // Base address
					break;
				case KeyAddressGroup::keys:
					data[0] = 0x12;  // Base address
					data[1] = 0xff;  // Base address
					data[2] = 0x0c;  // Base address
					data[3] = 0x3a;  // Base address
					data[4] = 0x00;  // Base address
					data[5] = 0x01;  // Base address
					data[6] = 0x00;  // Base address
					data[7] = 0x0e;  // Base address
					break;
				default:
					return false;
					break;
			}
			break;
		case KeyboardProtocol::g910: // gkeys and mkeys seem not changeable
			switch (addressGroup) {
				case KeyAddressGroup::logo:
					data[0] = 0x11;  // Base address
					data[1] = 0xff;  // Base address
					data[2] = 0x0f;  // Base address
					data[3] = 0x3a;  // Base address
					data[4] = 0x00;  // Base address
					data[5] = 0x10;  // Base address
					data[6] = 0x00;  // Base address
					data[7] = 0x02;  // Base address
					break;
				case KeyAddressGroup::indicators:
					data[0] = 0x12;  // Base address
					data[1] = 0xff;  // Base address
					data[2] = 0x0c;  // Base address
					data[3] = 0x3a;  // Base address
					data[4] = 0x00;  // Base address
					data[5] = 0x40;  // Base address
					data[6] = 0x00;  // Base address
					data[7] = 0x05;  // Base address
					break;
				case KeyAddressGroup::keys:
					data[0] = 0x12;  // Base address
					data[1] = 0xff;  // Base address
					data[2] = 0x0f;  // Base address
					data[3] = 0x3d;  // Base address
					data[4] = 0x00;  // Base address
					data[5] = 0x01;  // Base address
					data[6] = 0x00;  // Base address
					data[7] = 0x0e;  // Base address
					break;
				case KeyAddressGroup::gkeys:
					data[0] = 0x12;  // Base address
					data[1] = 0xff;  // Base address
					data[2] = 0x0f;  // Base address
					data[3] = 0x3e;  // Base address
					data[4] = 0x00;  // Base address
					data[5] = 0x04;  // Base address
					data[6] = 0x00;  // Base address
					data[7] = 0x09;  // Base address
					break;
				default:
					return false;
					break;
			}
			break;
		default:
			return false;
			break;
	}
	return true;
}

bool Keyboard::setKeysInternal(KeyAddressGroup addressGroup, KeyValue keyValues[], size_t keyValueCount) {
	bool retval = false;
	int data_size;
	if (addressGroup == KeyAddressGroup::logo) data_size = 20;
	else data_size = 64;
	unsigned char *data = new unsigned char[data_size];
	const size_t maxKeyValueCount = (data_size - 8) / 4;
	populateAddressGroupInternal(addressGroup, data);
	for(size_t i = 0; i < maxKeyValueCount; i++) {
		if (i < keyValueCount) {
			data[8 + i * 4 + 0] = keyValues[i].key.id;
			data[8 + i * 4 + 1] = keyValues[i].colors.red;
			data[8 + i * 4 + 2] = keyValues[i].colors.green;
			data[8 + i * 4 + 3] = keyValues[i].colors.blue;
		} else {
			data[8 + i * 4 + 0] = 0x00;
			data[8 + i * 4 + 1] = 0x00;
			data[8 + i * 4 + 2] = 0x00;
			data[8 + i * 4 + 3] = 0x00;
		}
	}
	retval = sendDataInternal(data, data_size);
	delete[] data;
	return retval;
}

bool Keyboard::setPowerOnEffect(PowerOnEffect powerOnEffect) {
	bool retval = false;
	int data_size = 20;
	unsigned char *data = new unsigned char[data_size];
	data[0] = 0x11;  // Base address
	data[1] = 0xff;  // Base address
	data[2] = 0x0d;  // Base address
	data[3] = 0x5a;  // Base address
	data[4] = 0x00;  // Base address
	data[5] = 0x01;  // Base address
	switch (powerOnEffect) {
		case PowerOnEffect::rainbow:
			data[6] = 0x01;
			break;
		case PowerOnEffect::color:
			data[6] = 0x02;
			break;
	}
	for(int i = 7; i < data_size; i++) data[i] = 0x00;
	retval = sendDataInternal(data, data_size);
	delete[] data;
	return retval;
}

bool Keyboard::setKey(KeyValue keyValue) {
	bool retval = false;
	unsigned char *data;
	int data_size;
	if (keyValue.key.addressGroup == KeyAddressGroup::logo) {
		data_size = 20;
		data = new unsigned char[data_size];
		populateAddressGroupInternal(keyValue.key.addressGroup, data);
	} else {
		data_size = 64;
		data = new unsigned char[data_size];
		populateAddressGroupInternal(keyValue.key.addressGroup, data);
	}
	data[8] = keyValue.key.id;
	data[9] = keyValue.colors.red;
	data[10] = keyValue.colors.green;
	data[11] = keyValue.colors.blue;
	for(int i = 12; i < data_size; i++) data[i] = 0x00;
	retval = sendDataInternal(data, data_size);
	delete[] data;
	return retval;
}

bool Keyboard::setKey(Key key, KeyColors colors) {
	KeyValue keyValue;
	getKeyAddress(key, keyValue.key);
	keyValue.colors = colors;
	return setKey(keyValue);
}

bool Keyboard::setKeys(KeyValue keyValue[], size_t keyValueCount) {
	const size_t maxLogoKeys = 5;
	const size_t maxIndicatorsKeys = 25;
	const size_t maxMultimediaKeys = 25;
	const size_t maxKeys = 200;
	const size_t maxGKeys = 25;
	size_t logoCount = 0;
	size_t indicatorsCount = 0;
	size_t multimediaCount = 0;
	size_t keysCount = 0;
	size_t gkeysCount = 0;
	KeyValue logo[maxLogoKeys];
	KeyValue indicators[maxIndicatorsKeys];
	KeyValue multimedia[maxMultimediaKeys];
	KeyValue keys[maxKeys];
	KeyValue gkeys[maxGKeys];
	
	for (size_t i = 0; i < keyValueCount; i++) {
		if(keyValue[i].key.addressGroup == KeyAddressGroup::logo && logoCount <= maxLogoKeys) {
			logo[logoCount] = keyValue[i];
			logoCount++;
		} else if(keyValue[i].key.addressGroup == KeyAddressGroup::indicators && indicatorsCount <= maxIndicatorsKeys) {
			indicators[indicatorsCount] = keyValue[i];
			indicatorsCount++;
		} else if(keyValue[i].key.addressGroup == KeyAddressGroup::multimedia && multimediaCount <= maxMultimediaKeys) {
			multimedia[multimediaCount] = keyValue[i];
			multimediaCount++;
		} else if(keyValue[i].key.addressGroup == KeyAddressGroup::keys && keysCount <= maxKeys) {
			keys[keysCount] = keyValue[i];
			keysCount++;
		} else if(keyValue[i].key.addressGroup == KeyAddressGroup::gkeys && gkeysCount <= maxGKeys) {
			gkeys[gkeysCount] = keyValue[i];
			gkeysCount++;
		}
	}
	
	if (logoCount > 0) setKeysInternal(KeyAddressGroup::logo, logo, logoCount);
	
	if (indicatorsCount > 0) setKeysInternal(KeyAddressGroup::indicators, indicators, indicatorsCount);
	
	if (multimediaCount > 0) setKeysInternal(KeyAddressGroup::multimedia, multimedia, multimediaCount);
	
	if (keysCount > 0) {
		const size_t maxKeyValueCount = 14;
		for (size_t i = 0; i < keysCount; i = i + maxKeyValueCount) {
			KeyValue keysBlock[maxKeyValueCount];
			size_t keysBlockCount = 0;
			for (size_t j = 0; j < maxKeyValueCount; j++) {
				if((i + j) < keysCount ) {
					keysBlock[j] = keys[i + j];
					keysBlockCount++;
				}
			}
			setKeysInternal(KeyAddressGroup::keys, keysBlock, keysBlockCount);
		}
	}
	
	if (gkeysCount > 0) setKeysInternal(KeyAddressGroup::gkeys, gkeys, gkeysCount);
	
	return true;
}

bool Keyboard::setAllKeys(KeyColors colors) {
	KeyValue keyValues[127];
	for (int i = 0; i < 127; i++) {
		getKeyAddress((Key)i, keyValues[i].key);
		keyValues[i].colors = colors;
	}
	setKeys(keyValues, 127);
	return true;
}

bool Keyboard::setGroupKeys(KeyGroup keyGroup, KeyColors colors) {
	KeyValue keyValues[54];
	int keyValuesCount = 0;
	switch (keyGroup) {
		case KeyGroup::logo:
			for (int i = 0; i < 2; i++) {
				getKeyAddress((Key)i, keyValues[i].key);
				keyValues[i].colors = colors;
				keyValuesCount++;
			}
			setKeys(keyValues, keyValuesCount);
			break;
		case KeyGroup::indicators:
			for (int i = 2; i < 7; i++) {
				getKeyAddress((Key)i, keyValues[i - 2].key);
				keyValues[i - 2].colors = colors;
				keyValuesCount++;
			}
			setKeys(keyValues, keyValuesCount);
			break;
		case KeyGroup::multimedia:
			for (int i = 7; i < 12; i++) {
				getKeyAddress((Key)i, keyValues[i - 7].key);
				keyValues[i - 7].colors = colors;
				keyValuesCount++;
			}
			setKeys(keyValues, keyValuesCount);
			break;
		case KeyGroup::fkeys:
			for (int i = 12; i < 24; i++) {
				getKeyAddress((Key)i, keyValues[i - 12].key);
				keyValues[i - 12].colors = colors;
				keyValuesCount++;
			}
			setKeys(keyValues, keyValuesCount);
			break;
		case KeyGroup::modifiers:
			for (int i = 24; i < 33; i++) {
				getKeyAddress((Key)i, keyValues[i - 24].key);
				keyValues[i - 24].colors = colors;
				keyValuesCount++;
			}
			setKeys(keyValues, keyValuesCount);
			break;
		case KeyGroup::arrows:
			for (int i = 33; i < 37; i++) {
				getKeyAddress((Key)i, keyValues[i - 33].key);
				keyValues[i - 33].colors = colors;
				keyValuesCount++;
			}
			setKeys(keyValues, keyValuesCount);
			break;
		case KeyGroup::numeric:
			for (int i = 37; i < 54; i++) {
				getKeyAddress((Key)i, keyValues[i - 37].key);
				keyValues[i - 37].colors = colors;
				keyValuesCount++;
			}
			setKeys(keyValues, keyValuesCount);
			break;
		case KeyGroup::functions:
			for (int i = 54; i < 64; i++) {
				getKeyAddress((Key)i, keyValues[i - 54].key);
				keyValues[i - 54].colors = colors;
				keyValuesCount++;
			}
			setKeys(keyValues, keyValuesCount);
			break;
		case KeyGroup::keys:
			for (int i = 64; i < 118; i++) {
				getKeyAddress((Key)i, keyValues[i - 64].key);
				keyValues[i - 64].colors = colors;
				keyValuesCount++;
			}
			setKeys(keyValues, keyValuesCount);
			break;
		case KeyGroup::gkeys:
			for (int i = 118; i < 127; i++) {
				getKeyAddress((Key)i, keyValues[i - 118].key);
				keyValues[i - 118].colors = colors;
				keyValuesCount++;
			}
			setKeys(keyValues, keyValuesCount);
			break;
	}
	return true;
}
