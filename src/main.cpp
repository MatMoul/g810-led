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

int setMRKey(LedKeyboard &kbd, std::string arg2, bool commit = true) {
	uint8_t value;
	if (! utils::parseUInt8(arg2, value)) return 1;
	if (! kbd.open()) return 1;
	if (! kbd.setMRKey(value)) return 1;
	if (commit) if(! kbd.commit()) return 1;
	return 0;
}

int setMNKey(LedKeyboard &kbd, std::string arg2, bool commit = true) {
	uint8_t value;
	if (! utils::parseUInt8(arg2, value)) return 1;
	if (! kbd.open()) return 1;
	if (! kbd.setMNKey(value)) return 1;
	if (commit) if(! kbd.commit()) return 1;
	return 0;
}


int setFX(LedKeyboard &kbd, std::string arg2, std::string arg3, std::string arg4, std::string arg5 = "") {
	LedKeyboard::NativeEffect effect;
	LedKeyboard::NativeEffectPart effectPart;
	uint8_t speed = 0;
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
			if (! utils::parseSpeed(arg5, speed)) return 1;
			break;
		case LedKeyboard::NativeEffect::cycle:
		case LedKeyboard::NativeEffect::hwave:
		case LedKeyboard::NativeEffect::vwave:
		case LedKeyboard::NativeEffect::cwave:
			if (! utils::parseSpeed(arg4, speed)) return 1;
			break;
	}
	
	if (! kbd.open()) return 1;
	
	int retval = 0;
	
	switch (effectPart) {
		case LedKeyboard::NativeEffectPart::all:
			switch (effect) {
				case LedKeyboard::NativeEffect::color:
					if (! kbd.setGroupKeys(LedKeyboard::KeyGroup::indicators, color)) retval = 1;
					if (! kbd.commit()) retval = 1;
					break;
				case LedKeyboard::NativeEffect::breathing:
					if (! kbd.setGroupKeys(LedKeyboard::KeyGroup::indicators, color)) retval = 1;
					if (! kbd.commit()) retval = 1;
					break;
				case LedKeyboard::NativeEffect::cycle:
				case LedKeyboard::NativeEffect::hwave:
				case LedKeyboard::NativeEffect::vwave:
				case LedKeyboard::NativeEffect::cwave:
					if (! kbd.setGroupKeys(
						LedKeyboard::KeyGroup::indicators, 
						LedKeyboard::Color({0xff, 0xff, 0xff}))
					) retval = 1;
					if (! kbd.commit()) retval = 1;
					break;
			}
			if (! kbd.setNativeEffect(effect, LedKeyboard::NativeEffectPart::keys, speed, color)) retval = 1;
			if (! kbd.setNativeEffect(effect, LedKeyboard::NativeEffectPart::logo, speed, color)) retval = 1;
			break;
		default:
			if (! kbd.setNativeEffect(effect, effectPart, speed, color)) retval = 1;
			break;
	}
	
	return retval;
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
	if (argc > 1) {
		
		std::string arg = argv[1];
		
		if (arg == "--help" || arg == "-h") help::usage(argv[0]);
		else if (arg == "--help-keys") help::keys(argv[0]);
		else if (arg == "--help-effects") help::effects(argv[0]);
		else if (arg == "--help-samples") help::samples(argv[0]);
		
		else {
			LedKeyboard kbd;
			if (arg == "--list-keyboards") kbd.listKeyboards();
			else if (arg == "-c") return commit(kbd);
			
			else if (argc > 2 && arg == "-a") return setAllKeys(kbd, argv[2]);
			else if (argc > 3 && arg == "-g") return setGroupKeys(kbd, argv[2], argv[3]);
			else if (argc > 3 && arg == "-k") return setKey(kbd, argv[2], argv[3]);
			else if (argc > 2 && arg == "-mr") return setMRKey(kbd, argv[2]);
			else if (argc > 2 && arg == "-mn") return setMNKey(kbd, argv[2]);
			else if (argc > 2 && arg == "-an") return setAllKeys(kbd, argv[2], false);
			else if (argc > 3 && arg == "-gn") return setGroupKeys(kbd, argv[2], argv[3], false);
			else if (argc > 3 && arg == "-kn") return setKey(kbd, argv[2], argv[3], false);
			else if (argc > 2 && arg == "-mrn") return setMRKey(kbd, argv[2], false);
			else if (argc > 2 && arg == "-mnn") return setMNKey(kbd, argv[2], false);
			
			else if (argc > 2 && arg == "-p") return loadProfile(kbd, argv[2]);
			else if (argc > 1 && arg == "-pp") return pipeProfile(kbd);
			
			else if (argc > 5 && arg == "-fx") return setFX(kbd, argv[2], argv[3], argv[4], argv[5]);
			else if (argc > 4 && arg == "-fx") return setFX(kbd, argv[2], argv[3], argv[4]);
			
			else if (argc > 2 && arg == "--startup-mode") return setStartupMode(kbd, argv[2]);
			
			else { help::usage(argv[0]); return 1; }
		}
		
		return 0;
		
	}
	
	help::usage(argv[0]);
	return 1;
}
