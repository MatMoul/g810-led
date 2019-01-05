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

#include <iomanip>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <map>

#include "helpers/help.h"
#include "helpers/utils.h"
#include "classes/Keyboard.h"


int commit(LedKeyboard &kbd) {
	if (! kbd.open()) return 1;
	if (kbd.commit()) return 0;
	return 1;
}

void printDeviceInfo(LedKeyboard::DeviceInfo device) {
	std::cout<<"Device: "<<device.manufacturer<<" - "<<device.product<<std::endl;
	std::cout<<"\tVendor ID: "<<std::hex<<std::setw(4)<<std::setfill('0')<<device.vendorID<<std::endl;
	std::cout<<"\tProduct ID: "<<std::hex<<std::setw(4)<<std::setfill('0')<<device.productID<<std::endl;
	std::cout<<"\tSerial Number: "<<device.serialNumber<<std::endl;
}

int listKeyboards(LedKeyboard &kbd) {
	std::vector<LedKeyboard::DeviceInfo> deviceList = kbd.listKeyboards();
	if (deviceList.empty()) {
		std::cout<<"Matching or compatible device not found !"<<std::endl;
		return 1;
	}

	std::vector<LedKeyboard::DeviceInfo>::iterator iterator;
	for (iterator = deviceList.begin(); iterator != deviceList.end(); iterator++) {
		LedKeyboard::DeviceInfo device = *iterator;
		printDeviceInfo(device);
	}

	return 0;
}

int setAllKeys(LedKeyboard &kbd, std::string arg2, bool commit = true) {
	LedKeyboard::Color color;
	if (! utils::parseColor(arg2, color)) return 1;
	if (! kbd.open()) return 1;
	if(! kbd.setAllKeys(color)) return 1;
	if (commit) if(! kbd.commit()) return 1;
	return 0;
}

int setGroupKeys(LedKeyboard &kbd, std::string arg2, std::string arg3, bool commit = true) {
	LedKeyboard::KeyGroup keyGroup;
	LedKeyboard::Color color;
	if (! utils::parseKeyGroup(arg2, keyGroup)) return 1;
	if (! utils::parseColor(arg3, color)) return 1;
	if (! kbd.open()) return 1;
	if (! kbd.setGroupKeys(keyGroup, color)) return 1;
	if (commit) if(! kbd.commit()) return 1;
	return 0;
}

int setKey(LedKeyboard &kbd, std::string arg2, std::string arg3, bool commit = true) {
	LedKeyboard::Key key;
	LedKeyboard::Color color;
	if (! utils::parseKey(arg2, key)) return 1;
	if (! utils::parseColor(arg3, color)) return 1;
	LedKeyboard::KeyValue keyValue = { key, color };
	if (! kbd.open()) return 1;
	if (! kbd.setKey(keyValue)) return 1;
	if (commit) if(! kbd.commit()) return 1;
	return 0;
}

int setMRKey(LedKeyboard &kbd, std::string arg2) {
	uint8_t value;
	if (! utils::parseUInt8(arg2, value)) return 1;
	if (! kbd.open()) return 1;
	if (! kbd.setMRKey(value)) return 1;
	return 0;
}

int setMNKey(LedKeyboard &kbd, std::string arg2) {
	uint8_t value;
	if (! utils::parseUInt8(arg2, value)) return 1;
	if (! kbd.open()) return 1;
	if (! kbd.setMNKey(value)) return 1;
	return 0;
}

int setGKeysMode(LedKeyboard &kbd, std::string arg2) {
	uint8_t value;
	if (! utils::parseUInt8(arg2, value)) return 1;
	if (! kbd.open()) return 1;
	if (! kbd.setGKeysMode(value)) return 1;
	return 0;
}

int setRegion(LedKeyboard &kbd, std::string arg2, std::string arg3) {
	uint8_t region = 0;
	LedKeyboard::Color color;
	if (! utils::parseColor(arg3, color)) return 1;
	if (! utils::parseUInt8(arg2, region)) return 1;
	if (kbd.setRegion(region, color)) return 0;
	return 1;
}

int setFX(LedKeyboard &kbd, LedKeyboard::NativeEffectStorage storage,
	  const std::string &arg2, const std::string &arg3, const std::string &arg4, const std::string &arg5) {
	LedKeyboard::NativeEffect effect;
	LedKeyboard::NativeEffectPart effectPart;
	std::chrono::duration<uint16_t, std::milli> period(0);
	LedKeyboard::Color color;
	if (! utils::parseNativeEffect(arg2, effect)) return 1;
	if (! utils::parseNativeEffectPart(arg3, effectPart)) return 1;

	switch (effect) {
		case LedKeyboard::NativeEffect::color:
			if (! utils::parseColor(arg4, color)) return 1;
			break;
		case LedKeyboard::NativeEffect::breathing:
			if (! utils::parseColor(arg4, color)) return 1;
			if (arg5 == "") return 1;
			if (! utils::parsePeriod(arg5, period)) return 1;
			break;
		case LedKeyboard::NativeEffect::cycle:
		case LedKeyboard::NativeEffect::waves:
		case LedKeyboard::NativeEffect::hwave:
		case LedKeyboard::NativeEffect::vwave:
		case LedKeyboard::NativeEffect::cwave:
			if (! utils::parsePeriod(arg4, period)) return 1;
			break;
	}

	if (! kbd.open()) return 1;

	if (! kbd.setNativeEffect(effect, effectPart, period, color, storage)) return 1;

	return 0;
}

int setFX(LedKeyboard &kbd, const std::string &arg2, const std::string &arg3, const std::string &arg4 = std::string(),
	  const std::string &arg5 = std::string()) {
	return setFX(kbd, LedKeyboard::NativeEffectStorage::none, arg2, arg3, arg4, arg5);
}

int storeFX(LedKeyboard &kbd, const std::string &arg2, const std::string &arg3, const std::string &arg4 = std::string(),
	  const std::string &arg5 = std::string()) {
	return setFX(kbd, LedKeyboard::NativeEffectStorage::user, arg2, arg3, arg4, arg5);
}

int setStartupMode(LedKeyboard &kbd, std::string arg2) {
	LedKeyboard::StartupMode startupMode;
	if (! utils::parseStartupMode(arg2, startupMode)) return 1;
	if (! kbd.open()) return 1;
	if (kbd.setStartupMode(startupMode)) return 0;
	return 1;
}


int parseProfile(LedKeyboard &kbd, std::istream &stream) {
	std::string line;
	std::map<std::string, std::string> vars;
	LedKeyboard::KeyValueArray keys = {};
	int retval = 0;
	while (!stream.eof()) {
		getline(stream, line);
		if (line.size() > 0 && line.substr(0, 1) != "#") {
			std::vector<std::string> args = {};
			while (line.size() > 0) {
				uint32_t ind = line.find(" ");
				std::string argValue = line.substr(0, ind);
				if (argValue.substr(0, 1) == "$") argValue = vars[argValue.substr(1)];
				args.push_back(argValue);
				if (line.substr(0, ind) == line) line.clear();
				else line = line.substr(ind + 1);
			}
			if (args[0] == "var" && args.size() > 2) {
				vars[args[1]] = args[2];
			} else if (args[0] == "c") {
				if (kbd.open()) {
					if (keys.size() > 0) {
						if (! kbd.setKeys(keys)) retval = 1;
						keys.clear();
					}
					if(! kbd.commit()) retval = 1;
				} else retval = 1;
			} else if (args[0] == "a" && args.size() > 1) {
				if (setAllKeys(kbd, args[1], false) == 1) retval = 1;
			} else if (args[0] == "g" && args.size() > 2) {
				if (setGroupKeys(kbd, args[1], args[2], false) == 1) retval = 1;
			} else if (args[0] == "k" && args.size() > 2) {
				LedKeyboard::Key key;
				LedKeyboard::Color color;
				if (utils::parseKey(args[1], key))
					if (utils::parseColor(args[2], color))
						keys.push_back({ key, color });
			} else if (args[0] == "r" && args.size() > 2) {
				if (setRegion(kbd, args[1], args[2]) == 1) retval = 1;
			} else if (args[0] == "mr" && args.size() > 1) {
				if (setMRKey(kbd, args[1]) == 1) retval = 1;
			} else if (args[0] == "mn" && args.size() > 1) {
				if (setMNKey(kbd, args[1]) == 1) retval = 1;
			} else if (args[0] == "gkm" && args.size() > 1) {
				if (setGKeysMode(kbd, args[1]) == 1) retval = 1;
			} else if (args[0] == "fx" && args.size() > 4) {
				if (setFX(kbd, args[1], args[2], args[3], args[4]) == 1) retval = 1;
			} else if (args[0] == "fx" && args.size() > 3) {
				if (setFX(kbd, args[1], args[2], args[3]) == 1) retval = 1;
			}
		}
	}
	return retval;
}
	
int loadProfile(LedKeyboard &kbd, char *arg2) {
	std::ifstream file;
	file.open(arg2);
	if (file.is_open()) {
		int retval = 0;
		retval = parseProfile(kbd, file);
		file.close();
		return retval;
	}
	return 1;
}

int pipeProfile(LedKeyboard &kbd) {
	if (isatty(fileno(stdin))) return 1;
	return parseProfile(kbd, std::cin);
}



int main(int argc, char **argv) {
	if (argc < 2) {
		help::usage(argv[0]);
		return 1;
	}
		
	LedKeyboard kbd;
	std::string serial;
	uint16_t vendorID = 0x0;
	uint16_t productID = 0x0;

	int argIndex = 1;
	while (argIndex < argc)
	{
		std::string arg = argv[argIndex];

		// Non-Command arguments
		if (argc > (argIndex + 1) && arg == "-ds") {
			serial = argv[argIndex + 1];
			argIndex += 2;
			continue;
		} else if (argc > (argIndex + 1) && arg == "-dv"){
			if (! utils::parseUInt16(argv[argIndex + 1], vendorID)) return 1;
			argIndex += 2;
			continue;
		} else if (argc > (argIndex + 1) && arg == "-dp"){
			if (! utils::parseUInt16(argv[argIndex + 1], productID)) return 1;
			argIndex += 2;
			continue;
		} else if (argc > (argIndex + 1) && arg == "-tuk"){
			uint8_t kbdProtocol = 0;
			if (! utils::parseUInt8(argv[argIndex + 1], kbdProtocol)) return 1;
			switch(kbdProtocol) {
				case 1:
					kbd.SupportedKeyboards = { { vendorID, productID, (u_int16_t)LedKeyboard::KeyboardModel::g810 } };
					break;
				case 2:
					kbd.SupportedKeyboards = { { vendorID, productID, (u_int16_t)LedKeyboard::KeyboardModel::g910 } };
					break;
				case 3:
					kbd.SupportedKeyboards = { { vendorID, productID, (u_int16_t)LedKeyboard::KeyboardModel::g213 } };
					break;
				default:
					break;
			}
			argIndex += 2;
			continue;
		}
		

		//Commands that do not need to initialize a specific device
		if (arg == "--help" || arg == "-h") {help::usage(argv[0]); return 0;}
		else if (arg == "--list-keyboards") return listKeyboards(kbd);
		else if (arg == "--help-keys") {help::keys(argv[0]); return 0;}
		else if (arg == "--help-effects") {help::effects(argv[0]); return 0;}
		else if (arg == "--help-samples") {help::samples(argv[0]); return 0;}

		//Initialize the device for use
		if (!kbd.open(vendorID, productID, serial)) {
			std::cout << "Matching or compatible device not found !" << std::endl;
			return 2;
		}
		
		// Command arguments, these will cause parsing to ignore anything beyond the command and its arguments
		if (arg == "-c") return commit(kbd);
		else if (arg == "--print-device") {printDeviceInfo(kbd.getCurrentDevice()); return 0;}
		else if (argc > (argIndex + 1) && arg == "-a") return setAllKeys(kbd, argv[argIndex + 1]);
		else if (argc > (argIndex + 2) && arg == "-g") return setGroupKeys(kbd, argv[argIndex + 1], argv[argIndex + 2]);
		else if (argc > (argIndex + 2) && arg == "-k") return setKey(kbd, argv[argIndex + 1], argv[argIndex + 2]);
		else if (argc > (argIndex + 1) && arg == "-mr") return setMRKey(kbd, argv[argIndex + 1]);
		else if (argc > (argIndex + 1) && arg == "-mn") return setMNKey(kbd, argv[argIndex + 1]);
		else if (argc > (argIndex + 1) && arg == "-an") return setAllKeys(kbd, argv[argIndex + 1], false);
		else if (argc > (argIndex + 2) && arg == "-gn")
			return setGroupKeys(kbd, argv[argIndex + 1], argv[argIndex + 2], false);
		else if (argc > (argIndex + 2) && arg == "-kn") return setKey(kbd, argv[argIndex + 1], argv[argIndex + 2], false);
		else if (argc > (argIndex + 2) && arg == "-r") return setRegion(kbd, argv[argIndex + 1], argv[argIndex + 2]);
		else if (argc > (argIndex + 1) && arg == "-gkm") return setGKeysMode(kbd, argv[argIndex + 1]);
		else if (argc > (argIndex + 1) && arg == "-p") return loadProfile(kbd, argv[argIndex + 1]);
		else if (arg == "-pp") return pipeProfile(kbd);
		else if (argc > (argIndex + 4) && arg == "-fx")
			return setFX(kbd, argv[argIndex + 1], argv[argIndex + 2], argv[argIndex + 3], argv[argIndex + 4]);
		else if (argc > (argIndex + 3) && arg == "-fx")
			return setFX(kbd, argv[argIndex + 1], argv[argIndex + 2], argv[argIndex + 3]);
		else if (argc > (argIndex + 4) && arg == "-fx-store")
			return storeFX(kbd, argv[argIndex + 1], argv[argIndex + 2], argv[argIndex + 3], argv[argIndex + 4]);
		else if (argc > (argIndex + 3) && arg == "-fx-store")
			return storeFX(kbd, argv[argIndex + 1], argv[argIndex + 2], argv[argIndex + 3]);
		else if (argc > (argIndex + 1) && arg == "--startup-mode") return setStartupMode(kbd, argv[argIndex + 1]);
		else { help::usage(argv[0]); return 1; }
	}

	return 0;
}
