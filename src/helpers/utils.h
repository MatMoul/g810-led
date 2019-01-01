#ifndef UTILS_HELPER
#define UTILS_HELPER

#include <chrono>
#include <iostream>
#include "../classes/Keyboard.h"

namespace utils {
	
	std::string getCmdName(std::string cmd);
	
	bool parseStartupMode(std::string val, LedKeyboard::StartupMode &startupMode);
	bool parseNativeEffect(std::string val, LedKeyboard::NativeEffect &nativeEffect);
	bool parseNativeEffectPart(std::string val, LedKeyboard::NativeEffectPart &nativeEffectPart);
	bool parseKey(std::string val, LedKeyboard::Key &key);
	bool parseKeyGroup(std::string val, LedKeyboard::KeyGroup &keyGroup);
	bool parseColor(std::string val, LedKeyboard::Color &color);
	bool parsePeriod(std::string val, std::chrono::duration<uint16_t, std::milli> &period);
	bool parseUInt8(std::string val, uint8_t &uint8);
	bool parseUInt16(std::string val, uint16_t &uint16);
	
}

#endif
