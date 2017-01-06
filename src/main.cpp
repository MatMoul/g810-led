#include <map>
#include <vector>
#include <fstream>
#include <istream>
#include <unistd.h>
#include "classes/Keyboard.h"

using namespace std;

string appname = "g810-led";

void usage() {
	cout<<appname<<" Usages :"<<endl;
	cout<<"-----------------"<<endl;
	cout<<""<<endl;
	cout<<"  -s  effect :\t\t\tSet keyboard startup effect"<<endl;
	cout<<""<<endl;
	cout<<"  -a  color :\t\t\tSet all keys"<<endl;
	cout<<"  -g  group, color :\t\tSet a group of keys"<<endl;
	cout<<"  -k  key, color :\t\tSet a key"<<endl;
	cout<<""<<endl;
	cout<<"  -an color :\t\t\tSet all keys without commit"<<endl;
	cout<<"  -gn group, color :\t\tSet a group of keys without commit"<<endl;
	cout<<"  -kn key, color :\t\tSet a key without commit"<<endl;
	cout<<""<<endl;
	cout<<"  -c :\t\t\t\tCommit changes"<<endl;
	cout<<""<<endl;
	cout<<"  -fx-color color :\t\tSet static color effect"<<endl;
	cout<<"  -fx-breathing color, speed :\tSet breathing effect"<<endl;
	cout<<"  -fx-cycle speed :\t\tSet color cycle effect"<<endl;
	cout<<"  -fx-hwave speed :\t\tSet horizontal color wave effect"<<endl;
	cout<<"  -fx-vwave speed :\t\tSet vertical color wave effect"<<endl;
	cout<<"  -fx-cwave speed :\t\tSet center color wave effect"<<endl;
	cout<<""<<endl;
	cout<<"  -p  profilefile :\t\tLoad a profile from a file"<<endl;
	cout<<"  -pp profilepipe :\t\tLoad a profile from stdin"<<endl;
	cout<<""<<endl;
	cout<<"  -h | --help :\t\t\tthis help message"<<endl;
	cout<<"  -lk | --list-keys :\t\tList keys in groups"<<endl;
	cout<<""<<endl;
	cout<<"color formats :\t\t\tRRGGBB (hex value for red, green and blue)"<<endl;
	cout<<"speed formats :\t\t\tSS (hex value for speed)"<<endl;
	cout<<""<<endl;
	cout<<"effect values :\t\t\trainbow, color"<<endl;
	cout<<"key values :\t\t\tabc... 123... and other (use -lk for more detail)"<<endl;
	cout<<"group values :\t\t\tlogo, indicators, fkeys, modifiers, multimedia, arrows, numeric, functions, keys, gkeys"<<endl;
	cout<<""<<endl;
	cout<<"samples :"<<endl;
	cout<<appname<<" -k logo ff0000"<<endl;
	cout<<appname<<" -a 00ff00"<<endl;
	cout<<appname<<" -g fkeys ff00ff"<<endl;
	cout<<appname<<" -s color"<<endl;
	cout<<appname<<" -fx-cycle 10"<<endl;
	cout<<appname<<" -p profilefile"<<endl;
	cout<<""<<endl;
	cout<<"samples with pipe:"<<endl;
	cout<<appname<<" -pp < profilefile"<<endl;
	cout<<"echo -e \"k w ff0000\\nk a ff0000\\nk s ff0000\\nk d ff0000\\nc\" | "<<appname<<" -pp"<<endl;
}

void listkeys() {
	cout<<appname<<" Keys in groups :"<<endl;
	cout<<"-------------------------"<<endl;
	cout<<""<<endl;
	cout<<"Group logo :"<<endl;
	cout<<" logo"<<endl;
	cout<<" logo2"<<endl;
	cout<<""<<endl;
	cout<<"Group indicators :"<<endl;
	cout<<" num_indicator, numindicator, num"<<endl;
	cout<<" caps_indicator, capsindicator, caps"<<endl;
	cout<<" scroll_indicator, scrollindicator, scroll"<<endl;
	cout<<" game_mode, gamemode, game"<<endl;
	cout<<" back_light, backlight, light"<<endl;
	cout<<""<<endl;
	cout<<"Group fkeys :"<<endl;
	cout<<" f1 - f12"<<endl;
	cout<<""<<endl;
	cout<<"Group modifiers :"<<endl;
	cout<<" shift_left, shiftleft, shiftl"<<endl;
	cout<<" ctrl_left, ctrlleft, ctrll"<<endl;
	cout<<" win_left, winleft, win_left"<<endl;
	cout<<" alt_left, altleft, altl"<<endl;
	cout<<" alt_right, altright, altr, altgr"<<endl;
	cout<<" win_right, winright, winr"<<endl;
	cout<<" menu"<<endl;
	cout<<" ctrl_right, ctrlright, ctrlr"<<endl;
	cout<<" shift_right, shiftright, shiftr"<<endl;
	cout<<""<<endl;
	cout<<"Group multimedia :"<<endl;
	cout<<" mute"<<endl;
	cout<<" play_pause, playpause, play"<<endl;
	cout<<" stop"<<endl;
	cout<<" previous, prev"<<endl;
	cout<<" next"<<endl;
	cout<<""<<endl;
	cout<<"Group arrows :"<<endl;
	cout<<" arrow_top, arrowtop, top"<<endl;
	cout<<" arrow_left, arrowleft, left"<<endl;
	cout<<" arrow_bottom, arrowbottom, bottom"<<endl;
	cout<<" arrow_right, arrowright, right"<<endl;
	cout<<""<<endl;
	cout<<"Group numeric :"<<endl;
	cout<<" num_lock, numlock"<<endl;
	cout<<" num_slash, numslash, num/"<<endl;
	cout<<" num_asterisk, numasterisk, num*"<<endl;
	cout<<" num_minus, numminus, num-"<<endl;
	cout<<" num_plus, numplus, num+"<<endl;
	cout<<" numenter"<<endl;
	cout<<" num0 - num9"<<endl;
	cout<<" num_dot, numdot, num."<<endl;
	cout<<""<<endl;
	cout<<"Group functions :"<<endl;
	cout<<" escape, esc"<<endl;
	cout<<" print_screen, printscreen, printscr"<<endl;
	cout<<" scroll_lock, scrolllock"<<endl;
	cout<<" pause_break, pausebreak"<<endl;
	cout<<" insert, ins"<<endl;
	cout<<" home"<<endl;
	cout<<" page_up, pageup"<<endl;
	cout<<" delete, del"<<endl;
	cout<<" end"<<endl;
	cout<<" page_down, pagedown"<<endl;
	cout<<""<<endl;
	cout<<"Group keys :"<<endl;
	cout<<" 0 - 9"<<endl;
	cout<<" a - z"<<endl;
	cout<<" tab"<<endl;
	cout<<" caps_lock, capslock"<<endl;
	cout<<" space"<<endl;
	cout<<" backspace, back"<<endl;
	cout<<" enter"<<endl;
	cout<<" tilde"<<endl;
	cout<<" minus"<<endl;
	cout<<" equal"<<endl;
	cout<<" open_bracket"<<endl;
	cout<<" close_bracket"<<endl;
	cout<<" backslash"<<endl;
	cout<<" semicolon"<<endl;
	cout<<" dollar"<<endl;
	cout<<" quote"<<endl;
	cout<<" intl_backslash"<<endl;
	cout<<" comma"<<endl;
	cout<<" period"<<endl;
	cout<<" slash"<<endl;
	cout<<"Group gkeys :"<<endl;
	cout<<" g1 - g9"<<endl;
}

int commit() {
	Keyboard lg_kbd;
	lg_kbd.attach();
	lg_kbd.commit();
	lg_kbd.detach();
	
	return 0;
}

int setStartupEffect(string effect) {
	Keyboard lg_kbd;
	Keyboard::PowerOnEffect powerOnEffect;
	if (lg_kbd.parsePowerOnEffect(effect, powerOnEffect) == true) {
		lg_kbd.attach();
		lg_kbd.setPowerOnEffect(powerOnEffect);
		lg_kbd.commit();
		lg_kbd.detach();
		return 0;
	}
	return 1;
}

int setKey(string key, string color, bool commit) {
	Keyboard lg_kbd;
	Keyboard::KeyAddress keyAddress;
	if (lg_kbd.parseKey(key, keyAddress) == true) {
		Keyboard::KeyColors colors;
		if (lg_kbd.parseColor(color, colors) == true) {
			Keyboard::KeyValue keyValue;
			keyValue.key = keyAddress;
			keyValue.colors = colors;
			lg_kbd.attach();
			lg_kbd.setKey(keyValue);
			if (commit == true) lg_kbd.commit();
			lg_kbd.detach();
			return 0;
		}
	}
	return 1;
}

int setAllKeys(string color, bool commit) {
	Keyboard lg_kbd;
	Keyboard::KeyColors colors;
	if (lg_kbd.parseColor(color, colors) == true) {
		lg_kbd.attach();
		lg_kbd.setAllKeys(colors);
		if (commit == true) lg_kbd.commit();
		lg_kbd.detach();
		return 0;
	}
	return 1;
}

int setGroupKeys(string groupKeys, string color, bool commit) {
	Keyboard lg_kbd;
	Keyboard::KeyGroup keyGroup;
	if (lg_kbd.parseKeyGroup(groupKeys, keyGroup) == true) {
		Keyboard::KeyColors colors;
		if (lg_kbd.parseColor(color, colors) == true) {
			lg_kbd.attach();
			lg_kbd.setGroupKeys(keyGroup, colors);
			if (commit == true) lg_kbd.commit();
			lg_kbd.detach();
			return 0;
		}
	}
	return 1;
}

int setFXColor(string color) {
	Keyboard lg_kbd;
	Keyboard::KeyColors colors;
	if (lg_kbd.parseColor(color, colors) == true) {
		lg_kbd.attach();
		lg_kbd.setGroupKeys(Keyboard::KeyGroup::indicators, colors);
		lg_kbd.commit();
		lg_kbd.detach();
		lg_kbd.attach();
		lg_kbd.setFXColor(colors);
		lg_kbd.detach();
		return 0;
	}
	return 1;
}

int setFXBreathing(string color, string speed) {
	Keyboard lg_kbd;
	Keyboard::KeyColors colors;
	uint8_t speedValue;
	if (lg_kbd.parseColor(color, colors) == true) {
		if (lg_kbd.parseSpeed(speed, speedValue) == true) {
			lg_kbd.attach();
			lg_kbd.setGroupKeys(Keyboard::KeyGroup::indicators, colors);
			lg_kbd.commit();
			lg_kbd.detach();
			lg_kbd.attach();
			lg_kbd.setFXBreathing(colors, speedValue);
			lg_kbd.detach();
			return 0;
		}
	}
	return 1;
}

int setFXColorCycle(string speed) {
	Keyboard lg_kbd;
	Keyboard::KeyColors colors;
	uint8_t speedValue;
	if (lg_kbd.parseSpeed(speed, speedValue) == true) {
		colors.red = 0xff;
		colors.green = 0xff;
		colors.blue = 0xff;
		lg_kbd.attach();
		lg_kbd.setGroupKeys(Keyboard::KeyGroup::indicators, colors);
		lg_kbd.commit();
		lg_kbd.detach();
		lg_kbd.attach();
		lg_kbd.setFXColorCycle(speedValue);
		lg_kbd.detach();
		return 0;
	}
	return 1;
}

int setFXHWave(string speed) {
	Keyboard lg_kbd;
	Keyboard::KeyColors colors;
	uint8_t speedValue;
	if (lg_kbd.parseSpeed(speed, speedValue) == true) {
		colors.red = 0xff;
		colors.green = 0xff;
		colors.blue = 0xff;
		lg_kbd.attach();
		lg_kbd.setGroupKeys(Keyboard::KeyGroup::indicators, colors);
		lg_kbd.commit();
		lg_kbd.detach();
		lg_kbd.attach();
		lg_kbd.setFXHWave(speedValue);
		lg_kbd.detach();
		return 0;
	}
	return 1;
}

int setFXVWave(string speed) {
	Keyboard lg_kbd;
	Keyboard::KeyColors colors;
	uint8_t speedValue;
	if (lg_kbd.parseSpeed(speed, speedValue) == true) {
		colors.red = 0xff;
		colors.green = 0xff;
		colors.blue = 0xff;
		lg_kbd.attach();
		lg_kbd.setGroupKeys(Keyboard::KeyGroup::indicators, colors);
		lg_kbd.commit();
		lg_kbd.detach();
		lg_kbd.attach();
		lg_kbd.setFXVWave(speedValue);
		lg_kbd.detach();
		return 0;
	}
	return 1;
}

int setFXCWave(string speed) {
	Keyboard lg_kbd;
	Keyboard::KeyColors colors;
	uint8_t speedValue;
	if (lg_kbd.parseSpeed(speed, speedValue) == true) {
		colors.red = 0xff;
		colors.green = 0xff;
		colors.blue = 0xff;
		lg_kbd.attach();
		lg_kbd.setGroupKeys(Keyboard::KeyGroup::indicators, colors);
		lg_kbd.commit();
		lg_kbd.detach();
		lg_kbd.attach();
		lg_kbd.setFXCWave(speedValue);
		lg_kbd.detach();
		return 0;
	}
	return 1;
}

int parseProfile(istream &is) {
	string line;
	int lineCount = 1;
	int ind;
	
	Keyboard lg_kbd;
	Keyboard::KeyGroup keyGroup;
	Keyboard::KeyAddress keyAddress;
	Keyboard::KeyValue keyValue;
	Keyboard::KeyColors colors;
	uint8_t speedValue;
	
	map<string, string> var;
	vector<Keyboard::KeyValue> keys;
	
	lg_kbd.attach();
	
	while (!is.eof()) {
		getline(is, line);
		
		if (line.substr(0, 3) == "var") {
			line = line.substr(4);
			ind = line.find(" ");
			var[line.substr(0, ind)] = line.substr(ind + 1, 6);
		} else if (line.substr(0, 1) == "a") {
			line = line.substr(2);
			if (line.substr(0, 1) == "$") {
				ind = line.find(" ");
				line = var[line.substr(1, ind - 1)];
			} else line = line.substr(0, 6);
			if (lg_kbd.parseColor(line, colors) == true) {
				keys.clear();
				lg_kbd.setAllKeys(colors);
			} else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
		} else if (line.substr(0,1) == "g") {
			line = line.substr(2);
			ind = line.find(" ");
			if (lg_kbd.parseKeyGroup(line.substr(0, ind), keyGroup) == true) {
				line = line.substr(ind + 1);
				if (line.substr(0, 1) == "$") {
					ind = line.find(" ");
					line = var[line.substr(1, ind - 1)];
				};
				if (lg_kbd.parseColor(line.substr(0, 6), colors) == true) {
					lg_kbd.setGroupKeys(keyGroup, colors);
				} else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
			} else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
		} else if (line.substr(0,1) == "k") {
			line = line.substr(2);
			ind = line.find(" ");
			if (lg_kbd.parseKey(line.substr(0, ind), keyAddress) == true) {
				line = line.substr(ind + 1);
				if (line.substr(0, 1) == "$") {
					ind = line.find(" ");
					line = var[line.substr(1, ind - 1)];
				}
				if (lg_kbd.parseColor(line.substr(0, 6), colors) == true) {
					keyValue.key = keyAddress;
					keyValue.colors = colors;
					keys.push_back(keyValue);
				} else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
			} else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
		} else if (line.substr(0,1) == "c") {
			lg_kbd.commit();
			lg_kbd.setKeys(&keys[0], keys.size());
			keys.clear();
			lg_kbd.commit();
		} else if (line.substr(0,8) == "fx-color") {
			line = line.substr(9);
			if (line.substr(0, 1) == "$") {
				ind = line.find(" ");
				line = var[line.substr(1, ind - 1)];
			} else line = line.substr(0, 6);
			if (lg_kbd.parseColor(line, colors) == true) {
				keys.clear();
				lg_kbd.setGroupKeys(Keyboard::KeyGroup::indicators, colors);
				lg_kbd.commit();
				lg_kbd.detach();
				lg_kbd.attach();
				lg_kbd.setFXColor(colors);
			} else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
		} else if (line.substr(0,12) == "fx-breathing") {
			line = line.substr(13);
			if (line.substr(0, 1) == "$") {
				ind = line.find(" ");
				line = var[line.substr(1, ind - 1)] + " " + line.substr(ind + 1);
			}
			if (lg_kbd.parseColor(line.substr(0, 6), colors) == true) {
				ind = line.find(" ");
				line = line.substr(ind + 1, 2);
				if (lg_kbd.parseSpeed(line, speedValue) == true) {
					keys.clear();
					lg_kbd.setGroupKeys(Keyboard::KeyGroup::indicators, colors);
					lg_kbd.commit();
					lg_kbd.detach();
					lg_kbd.attach();
					lg_kbd.setFXBreathing(colors, speedValue);
				} else cout<<"Error1 on line "<<lineCount<<" : "<<line<<"\n";
			} else cout<<"Error2 on line "<<lineCount<<" : "<<line<<"\n";
		} else if (line.substr(0,8) == "fx-cycle") {
			line = line.substr(9);
			if (line.substr(0, 1) == "$") {
				ind = line.find(" ");
				line = var[line.substr(1, ind - 1)];
			} else line = line.substr(0, 2);
			if (lg_kbd.parseSpeed(line, speedValue) == true) {
				keys.clear();
				colors.red = 0xff;
				colors.green = 0xff;
				colors.blue = 0xff;
				lg_kbd.setGroupKeys(Keyboard::KeyGroup::indicators, colors);
				lg_kbd.commit();
				lg_kbd.detach();
				lg_kbd.attach();
				lg_kbd.setFXColorCycle(speedValue);
			} else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
		} else if (line.substr(0,8) == "fx-hwave") {
			line = line.substr(9);
			if (line.substr(0, 1) == "$") {
				ind = line.find(" ");
				line = var[line.substr(1, ind - 1)];
			} else line = line.substr(0, 2);
			if (lg_kbd.parseSpeed(line, speedValue) == true) {
				keys.clear();
				colors.red = 0xff;
				colors.green = 0xff;
				colors.blue = 0xff;
				lg_kbd.setGroupKeys(Keyboard::KeyGroup::indicators, colors);
				lg_kbd.commit();
				lg_kbd.detach();
				lg_kbd.attach();
				lg_kbd.setFXHWave(speedValue);
			} else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
		} else if (line.substr(0,8) == "fx-vwave") {
			line = line.substr(9);
			if (line.substr(0, 1) == "$") {
				ind = line.find(" ");
				line = var[line.substr(1, ind - 1)];
			} else line = line.substr(0, 2);
			if (lg_kbd.parseSpeed(line, speedValue) == true) {
				keys.clear();
				colors.red = 0xff;
				colors.green = 0xff;
				colors.blue = 0xff;
				lg_kbd.setGroupKeys(Keyboard::KeyGroup::indicators, colors);
				lg_kbd.commit();
				lg_kbd.detach();
				lg_kbd.attach();
				lg_kbd.setFXVWave(speedValue);
			} else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
		} else if (line.substr(0,8) == "fx-cwave") {
			line = line.substr(9);
			if (line.substr(0, 1) == "$") {
				ind = line.find(" ");
				line = var[line.substr(1, ind - 1)];
			} else line = line.substr(0, 2);
			if (lg_kbd.parseSpeed(line, speedValue) == true) {
				keys.clear();
				colors.red = 0xff;
				colors.green = 0xff;
				colors.blue = 0xff;
				lg_kbd.setGroupKeys(Keyboard::KeyGroup::indicators, colors);
				lg_kbd.commit();
				lg_kbd.detach();
				lg_kbd.attach();
				lg_kbd.setFXCWave(speedValue);
			} else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
		} else if ((line.substr(0, 1) != "#") && (line.substr(0, 1) != "")) {
			cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
		}
	
		lineCount++;
	}
	
	lg_kbd.detach();
	
	return 0;
}

int loadProfile(string profileFile) {
	ifstream file;
	file.open(profileFile);
	if (file.is_open()) {
		int retval = parseProfile(file);
		file.close();
		return retval;
	}
	return 1;
}

int pipeProfile() {
	if (isatty(fileno(stdin))) return 1;
	return parseProfile(cin);
}

int main(int argc, char *argv[]) {
	string str = argv[0];
	size_t split = str.find_last_of("/\\");
	str = str.substr(split + 1);
	appname = str;
	if (argc > 1) {
		string argCmd = argv[1];
		if (argCmd == "-h" || argCmd == "--help")             { usage(); return 0; }
		else if (argCmd == "-lk" || argCmd == "--list-keys")  { listkeys(); return 0; }
		else if (argCmd == "-s" && argc == 3)                 return setStartupEffect(argv[2]);
		else if (argCmd == "-a" && argc == 3)                 return setAllKeys(argv[2], true);
		else if (argCmd == "-an" && argc == 3)                return setAllKeys(argv[2], false);
		else if (argCmd == "-g" && argc == 4)                 return setGroupKeys(argv[2], argv[3], true);
		else if (argCmd == "-gn" && argc == 4)                return setGroupKeys(argv[2], argv[3], false);
		else if (argCmd == "-k" && argc == 4)                 return setKey(argv[2], argv[3], true);
		else if (argCmd == "-kn" && argc == 4)                return setKey(argv[2], argv[3], false);
		else if (argCmd == "-c" && argc == 2)                 return commit();
		else if (argCmd == "-p" && argc == 3)                 return loadProfile(argv[2]);
		else if (argCmd == "-pp" && argc == 2)                return pipeProfile();
		else if (argCmd == "-fx-color" && argc == 3)          return setFXColor(argv[2]);
		else if (argCmd == "-fx-breathing" && argc == 4)      return setFXBreathing(argv[2], argv[3]);
		else if (argCmd == "-fx-cycle" && argc == 3)          return setFXColorCycle(argv[2]);
		else if (argCmd == "-fx-hwave" && argc == 3)          return setFXHWave(argv[2]);
		else if (argCmd == "-fx-vwave" && argc == 3)          return setFXVWave(argv[2]);
		else if (argCmd == "-fx-cwave" && argc == 3)          return setFXCWave(argv[2]);
	}
	usage();
	return 1;
}
