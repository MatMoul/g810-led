#ifndef UTILS_HELPER
#define UTILS_HELPER

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
	bool parseSpeed(std::string val, uint8_t &speed);
	
}

#endif
