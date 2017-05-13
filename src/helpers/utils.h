#ifndef UTILS_HELPER
#define UTILS_HELPER

#include <iostream>
#include <string.h>

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
	bool parseUInt8(std::string val, uint8_t &uint8);
	bool parseUInt16(std::string val, uint16_t &uint16);
	bool parseUSeconds(std::string val, useconds_t &useconds);
	
	int commit(LedKeyboard &kbd);
	void printDeviceInfo(LedKeyboard::DeviceInfo device);
	int listKeyboards(LedKeyboard &kbd);
	int setAllKeys(LedKeyboard &kbd, std::string arg2, bool commit = true);
	int setGroupKeys(LedKeyboard &kbd, std::string arg2, std::string arg3, bool commit = true);
	int setKey(LedKeyboard &kbd, std::string arg2, std::string arg3, bool commit = true);
	int setMRKey(LedKeyboard &kbd, std::string arg2);
	int setMNKey(LedKeyboard &kbd, std::string arg2);
	int setGKeysMode(LedKeyboard &kbd, std::string arg2);
	int setRegion(LedKeyboard &kbd, std::string arg2, std::string arg3);
	int setFX(LedKeyboard &kbd, std::string arg2, std::string arg3, std::string arg4, std::string arg5 = "");
	int setStartupMode(LedKeyboard &kbd, std::string arg2);
	int parseProfile(LedKeyboard &kbd, std::istream &stream);
	int loadProfile(LedKeyboard &kbd, char *arg2);
	int pipeProfile(LedKeyboard &kbd);
	
}

#endif
