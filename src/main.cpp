#include "helpers/help.h"
#include "helpers/utils.h"
#include "helpers/daemonizer.h"
#include "classes/Keyboard.h"

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
		else if (arg == "--list-keyboards") return utils::listKeyboards(kbd);
		else if (arg == "--help-keys") {help::keys(argv[0]); return 0;}
		else if (arg == "--help-effects") {help::effects(argv[0]); return 0;}
		else if (arg == "--help-samples") {help::samples(argv[0]); return 0;}
		else if (argc > (argIndex + 1) && arg == "--daemon" ) {daemonizer::start(argv[0], argv[argIndex + 1]); return 0;}

		//Initialize the device for use
		if (!kbd.open(vendorID, productID, serial)) {
			std::cout << "Matching or compatible device not found !" << std::endl;
			return 2;
		}
		
		// Command arguments, these will cause parsing to ignore anything beyond the command and its arguments
		if (arg == "-c") return utils::commit(kbd);
		else if (arg == "--print-device") {utils::printDeviceInfo(kbd.getCurrentDevice()); return 0;}
		else if (argc > (argIndex + 1) && arg == "-a") return utils::setAllKeys(kbd, argv[argIndex + 1]);
		else if (argc > (argIndex + 2) && arg == "-g") return utils::setGroupKeys(kbd, argv[argIndex + 1], argv[argIndex + 2]);
		else if (argc > (argIndex + 2) && arg == "-k") return utils::setKey(kbd, argv[argIndex + 1], argv[argIndex + 2]);
		else if (argc > (argIndex + 1) && arg == "-mr") return utils::setMRKey(kbd, argv[argIndex + 1]);
		else if (argc > (argIndex + 1) && arg == "-mn") return utils::setMNKey(kbd, argv[argIndex + 1]);
		else if (argc > (argIndex + 1) && arg == "-an") return utils::setAllKeys(kbd, argv[argIndex + 1], false);
		else if (argc > (argIndex + 2) && arg == "-gn")
			return utils::setGroupKeys(kbd, argv[argIndex + 1], argv[argIndex + 2], false);
		else if (argc > (argIndex + 2) && arg == "-kn") return utils::setKey(kbd, argv[argIndex + 1], argv[argIndex + 2], false);
		else if (argc > (argIndex + 2) && arg == "-r") return utils::setRegion(kbd, argv[argIndex + 1], argv[argIndex + 2]);
		else if (argc > (argIndex + 1) && arg == "-gkm") return utils::setGKeysMode(kbd, argv[argIndex + 1]);
		else if (argc > (argIndex + 1) && arg == "-p") return utils::loadProfile(kbd, argv[argIndex + 1]);
		else if (arg == "-pp") return utils::pipeProfile(kbd);
		else if (argc > (argIndex + 4) && arg == "-fx")
			return utils::setFX(kbd, argv[argIndex + 1], argv[argIndex + 2], argv[argIndex + 3], argv[argIndex + 4]);
		else if (argc > (argIndex + 3) && arg == "-fx")
			return utils::setFX(kbd, argv[argIndex + 1], argv[argIndex + 2], argv[argIndex + 3]);
		else if (argc > (argIndex + 1) && arg == "--startup-mode") return utils::setStartupMode(kbd, argv[argIndex + 1]);
		else { help::usage(argv[0]); return 1; }
	}

	return 0;
}
