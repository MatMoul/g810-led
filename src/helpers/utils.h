/*
  This file is part of g810-led.

  g810-led is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 3 of the License.

  g810-led is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with g810-led.  If not, see <https://www.gnu.org/licenses/>.
*/

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
