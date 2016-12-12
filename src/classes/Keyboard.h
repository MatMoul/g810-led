#ifndef DEF_KEYBOARD
#define DEF_KEYBOARD

#include <iostream>
#include <libusb.h>

class Keyboard {
	
	public:
		
		enum class KeyboardProtocol { generic, g910 };
		enum class PowerOnEffect { rainbow, color };
		enum class KeyAddressGroup { logo, indicators, multimedia, keys, gkeys };
		enum class Key { // 127 items
			logo, logo2,
			caps, num, scroll, game, backlight,
			mute, play, stop, prev, next,
			f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
			shift_left, ctrl_left, win_left, alt_left, alt_right, win_right, ctrl_right, shift_right, menu,
			arrow_top, arrow_left, arrow_bottom, arrow_right,
			num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9, num_0, num_dot, num_enter, num_plus, num_minus, num_asterisk, num_slash, num_lock,
			esc, scroll_lock,
			insert, del, home, end, page_up, page_down, print_screen, pause_break,
			n1, n2, n3, n4, n5, n6, n7, n8, n9, n0,
			tab, caps_lock, space, backspace, enter,
			a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
			tilde, minus, equal, 
			open_bracket, close_bracket, backslash, 
			semicolon, quote, dollar,
			intl_backslash, comma, period, slash,
			g1, g2, g3, g4, g5, g6, g7, g8, g9
		};
		enum class KeyGroup { logo, indicators, multimedia, fkeys, modifiers, arrows, numeric, functions, keys, gkeys};
		
		struct KeyColors { uint8_t red; uint8_t green; uint8_t blue; };
		struct KeyAddress { KeyAddressGroup addressGroup; uint8_t id; };
		struct KeyValue { KeyAddress key; KeyColors colors; };
		
		bool isAttached();
		bool attach();
		bool detach();
		bool commit();
		bool getKeyAddress(Key key, KeyAddress &keyAddress);
		bool parsePowerOnEffect(std::string effect, PowerOnEffect &powerOnEffect);
		bool parseKey(std::string key, KeyAddress &keyAddress);
		bool parseKeyGroup(std::string key, KeyGroup &keyGroup);
		bool parseColor(std::string color, KeyColors &colors);
		bool setPowerOnEffect(PowerOnEffect powerOnEffect);
		bool setKey(KeyValue keyValue);
		bool setKey(Key key, KeyColors colors);
		bool setKeys(KeyValue keyValue[], size_t keyValueCount);
		bool setAllKeys(KeyColors colors);
		bool setGroupKeys(KeyGroup keyGroup, KeyColors colors);
		
		
	private:
		
		bool m_isAttached = false;
		bool m_isKernellDetached = false;
		KeyboardProtocol kbdProtocol = KeyboardProtocol::generic;
		libusb_device **devs;
		libusb_device_handle *dev_handle;
		libusb_context *ctx = NULL;
		
		bool populateAddressGroupInternal(KeyAddressGroup addressGroup, unsigned char *data);
		bool sendDataInternal(unsigned char *data, uint16_t data_size);
		bool setKeysInternal(KeyAddressGroup addressGroup, KeyValue keyValues[], size_t keyValueCount);
		
};

#endif
