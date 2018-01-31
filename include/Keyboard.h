#ifndef KEYBOARD_CLASS
#define KEYBOARD_CLASS

#include <iostream>
#include <vector>

#if defined(hidapi)
	#include "hidapi/hidapi.h"
#elif defined(libusb)
	#include "libusb-1.0/libusb.h"
#endif


class LedKeyboard {
	
	
	private:
		
		enum class KeyAddressGroup : uint8_t {
			logo = 0x00,
			indicators,
			multimedia,
			gkeys,
			keys
		};
		
		
	public:
		
		std::vector<std::vector<uint16_t>> SupportedKeyboards = {
			{ 0x46d, 0xc336, (u_int16_t)KeyboardModel::g213 },
			{ 0x46d, 0xc330, (u_int16_t)KeyboardModel::g410 },
			{ 0x46d, 0xc33a, (u_int16_t)KeyboardModel::g413 },
			{ 0x46d, 0xc333, (u_int16_t)KeyboardModel::g610 },
			{ 0x46d, 0xc338, (u_int16_t)KeyboardModel::g610 },
			{ 0x46d, 0xc331, (u_int16_t)KeyboardModel::g810 },
			{ 0x46d, 0xc337, (u_int16_t)KeyboardModel::g810 },
			{ 0x46d, 0xc32b, (u_int16_t)KeyboardModel::g910 },
			{ 0x46d, 0xc335, (u_int16_t)KeyboardModel::g910 },
			{ 0x46d, 0xc339, (u_int16_t)KeyboardModel::gpro }
		};
		
		enum class KeyboardModel : uint8_t {
			unknown = 0x00,
			g213,
			g410,
			g413,
			g610,
			g810,
			g910,
			gpro
		};
		enum class StartupMode : uint8_t {
			wave = 0x01,
			color
		};
		enum class NativeEffect : uint8_t {
			color = 0x01,
			breathing,
			cycle,
			hwave,
			vwave,
			cwave
		};
		enum class NativeEffectPart : uint8_t {
			all = 0xff,
			keys = 0x00,
			logo
		};
		enum class KeyGroup : uint8_t {
			logo = 0x00,
			indicators,
			multimedia,
			gkeys,
			fkeys,
			modifiers,
			functions,
			arrows,
			numeric,
			keys
		};
		enum class Key : uint16_t { // 127 items
			
			logo = static_cast<uint8_t>(KeyAddressGroup::logo) << 8 | 0x01,
			logo2,
			
			backlight = static_cast<uint8_t>(KeyAddressGroup::indicators) << 8| 0x01,
			game, caps, scroll, num, 
			
			next = static_cast<uint8_t>(KeyAddressGroup::multimedia) << 8 | 0xb5,
			prev, stop,
			play = static_cast<uint8_t>(KeyAddressGroup::multimedia) << 8 | 0xcd,
			mute = static_cast<uint8_t>(KeyAddressGroup::multimedia) << 8 | 0xe2,
			
			g1 = static_cast<uint8_t>(KeyAddressGroup::gkeys) << 8 | 0x01,
			g2, g3, g4, g5, g6, g7, g8, g9,
			
			a = static_cast<uint8_t>(KeyAddressGroup::keys) << 8 | 0x04,
			b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z,
			n1, n2, n3, n4, n5, n6, n7, n8, n9, n0,
			enter, esc, backspace, tab, space, minus, equal, open_bracket, close_bracket,
			backslash, dollar, semicolon, quote, tilde, comma, period, slash, caps_lock,
			f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
			print_screen, scroll_lock, pause_break, insert, home, page_up, del, end, page_down,
			arrow_right, arrow_left, arrow_bottom, arrow_top, num_lock, num_slash, num_asterisk,
			num_minus, num_plus, num_enter,
			num_1, num_2, num_3, num_4, num_5, num_6, num_7, num_8, num_9, num_0,
			num_dot, intl_backslash, menu,
			
			ctrl_left = static_cast<uint8_t>(KeyAddressGroup::keys) << 8 | 0xe0,
			shift_left, alt_left, win_left,
			ctrl_right, shift_right, alt_right, win_right
			
		};

		typedef struct {
			uint16_t vendorID = 0x0;
			uint16_t productID = 0x0;
			std::string manufacturer = "";
			std::string product = "";
			std::string serialNumber = "";
			KeyboardModel model;
		} DeviceInfo;
		
		struct Color {
			uint8_t red;
			uint8_t green;
			uint8_t blue;
		};
		struct KeyValue {
			LedKeyboard::Key key;
			LedKeyboard::Color color;
		};
		
		typedef std::vector<KeyValue> KeyValueArray;
		
		
		~LedKeyboard();
		
		
		std::vector<DeviceInfo> listKeyboards();
		
		bool isOpen();
		bool open();
		bool open(uint16_t vendorID, uint16_t productID, std::string serial);
		DeviceInfo getCurrentDevice();
		bool close();
		
		KeyboardModel getKeyboardModel();
		
		bool commit();
		
		bool setKey(KeyValue keyValue);
		bool setKeys(KeyValueArray keyValues);
		bool setGroupKeys(KeyGroup keyGroup, Color color);
		bool setAllKeys(Color color);
		
		bool setMRKey(uint8_t value);
		bool setMNKey(uint8_t value);
		bool setGKeysMode(uint8_t value);
		
		bool setRegion(uint8_t region, Color color);
		bool setStartupMode(StartupMode startupMode);
		
		bool setNativeEffect(NativeEffect effect, NativeEffectPart part, uint8_t speed, Color color);
		
		
	private:
		
		typedef std::vector<unsigned char> byte_buffer_t;
		typedef std::vector<Key> KeyArray;
		
		
		const KeyArray keyGroupLogo = { Key::logo, Key::logo2 };
		const KeyArray keyGroupIndicators = { Key::caps, Key::num, Key::scroll, Key::game, Key::backlight };
		const KeyArray keyGroupMultimedia = { Key::next, Key::prev, Key::stop, Key::play, Key::mute };
		const KeyArray keyGroupGKeys = { Key::g1, Key::g2, Key::g3, Key::g4, Key::g5, Key::g6, Key::g7, Key::g8, Key::g9 };
		const KeyArray keyGroupFKeys = {
			Key::f1, Key::f2, Key::f3, Key::f4, Key::f5, Key::f6, 
			Key::f7, Key::f8, Key::f9, Key::f10, Key::f11, Key::f12 
		};
		const KeyArray keyGroupModifiers = {
			Key::shift_left, Key::ctrl_left, Key::win_left, Key::alt_left, 
			Key::alt_right, Key::win_right, Key::ctrl_right, Key::shift_right, Key::menu };
		const KeyArray keyGroupFunctions = {
			Key::esc, Key::print_screen, Key::scroll_lock, Key::pause_break,
			Key::insert, Key::del, Key::home, Key::end, Key::page_up, Key::page_down
		};
		const KeyArray keyGroupArrows = { Key::arrow_top, Key::arrow_left, Key::arrow_bottom, Key::arrow_right };
		const KeyArray keyGroupNumeric = {
			Key::num_1, Key::num_2, Key::num_3, Key::num_4, Key::num_5,
			Key::num_6, Key::num_7, Key::num_8, Key::num_9, Key::num_0, 
			Key::num_dot, Key::num_enter, Key::num_plus, Key::num_minus,
			Key::num_asterisk, Key::num_slash, Key::num_lock
		};
		const KeyArray keyGroupKeys = {
			Key::a, Key::b, Key::c, Key::d, Key::e, Key::f, Key::g, Key::h, Key::i, Key::j, Key::k, Key::l, Key::m, 
			Key::n, Key::o, Key::p, Key::q, Key::r, Key::s, Key::t, Key::u, Key::v, Key::w, Key::x, Key::y, Key::z,
			Key::n1, Key::n2, Key::n3, Key::n4, Key::n5, Key::n6, Key::n7, Key::n8, Key::n9, Key::n0,
			Key::enter, Key::backspace, Key::tab, Key::space, Key::minus, Key::equal,
			Key::open_bracket, Key::close_bracket, Key::backslash, Key::dollar, Key::semicolon, Key::quote, Key::tilde,
			Key::comma, Key::period, Key::slash, Key::caps_lock, Key::intl_backslash
		};
		
		bool m_isOpen = false;
		DeviceInfo currentDevice;
		
		#if defined(hidapi)
			hid_device *m_hidHandle;
		#elif defined(libusb)
			bool m_isKernellDetached = false;
			libusb_device_handle *m_hidHandle;
			libusb_context *m_ctx = NULL;
		#endif
		
		
		bool sendDataInternal(byte_buffer_t &data);
		byte_buffer_t getKeyGroupAddress(KeyAddressGroup keyAddressGroup);
		
};

#endif
