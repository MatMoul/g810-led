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

#include "help.h"

#include <iostream>
#include "utils.h"


using namespace std;

namespace help {
	
	inline KeyboardFeatures operator|(KeyboardFeatures a, KeyboardFeatures b) {
		return static_cast<KeyboardFeatures>(static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
	}
	
	KeyboardFeatures getKeyboardFeatures(string cmdName) {
		if(cmdName == "g213-led") return KeyboardFeatures::g213;
		else if(cmdName == "g410-led") return KeyboardFeatures::g410;
		else if(cmdName == "g413-led") return KeyboardFeatures::g413;
		else if(cmdName == "g513-led") return KeyboardFeatures::g513;
		else if(cmdName == "g610-led") return KeyboardFeatures::g610;
		else if(cmdName == "g810-led") return KeyboardFeatures::g810;
		else if(cmdName == "g910-led") return KeyboardFeatures::g910;
		else if(cmdName == "gpro-led") return KeyboardFeatures::gpro;
		return KeyboardFeatures::all;
	}
	
	
	void usage(char *arg0) {
		string cmdName = utils::getCmdName(arg0);
		KeyboardFeatures features = getKeyboardFeatures(cmdName);
		cout<<cmdName<<endl;
		cout<<"--------"<<endl;
		cout<<"Version : "<<VERSION<<endl;
		cout<<endl;
		cout<<"Usage: "<<cmdName<<" [OPTIONS...] [command] (command arguments)"<<endl;
		cout<<"Commands:"<<endl;
		if((features | KeyboardFeatures::setall) == features)
			cout<<"  -a {color}\t\t\t\tSet all keys color"<<endl;
		if((features | KeyboardFeatures::setgroup) == features)
			cout<<"  -g {keygroup} {color}\t\t\tSet key group color"<<endl;
		if((features | KeyboardFeatures::setkey) == features)
			cout<<"  -k {key} {color}\t\t\tSet key color"<<endl;
		if((features | KeyboardFeatures::setregion) == features)
			cout<<"  -r {region} {color}\t\t\tSet the color of a region for a region-based keyboard"<<endl;
		if((features | KeyboardFeatures::gkeys) == features) {
			cout<<"  -mr {value}\t\t\t\tSet MR key (0-1)"<<endl;
			cout<<"  -mn {value}\t\t\t\tSet MN key (0-7) (M1=1, M2=2, M3=4) (M1+M2=3, M1+M3=5, ...)"<<endl;
			cout<<endl;
			cout<<"  -gkm {value}\t\t\t\tSet GKeys mode (0=Mapped to FKeys, 1=Independent)"<<endl;
		}
		cout<<endl;
		if((features | KeyboardFeatures::commit) == features) {
			if((features | KeyboardFeatures::setall) == features)
				cout<<"  -an {color}\t\t\t\tSet all keys color without commit"<<endl;
			if((features | KeyboardFeatures::setgroup) == features)
				cout<<"  -gn {keygroup} {color}\t\tSet key group color without commit"<<endl;
			if((features | KeyboardFeatures::setkey) == features)
				cout<<"  -kn {key} {color}\t\t\tSet key color without commit"<<endl;
			cout<<"  -c\t\t\t\t\tCommit change"<<endl;
			cout<<endl;
		}
		cout<<"  -fx ...\t\t\t\tActivate an on-board lighting effect"<<endl;
		if((features | KeyboardFeatures::userstoredlighting) == features)
			cout<<"  -fx-store ...\t\t\t\tSet an on-board effect as user-stored lighting"<<endl;
		cout<<"               \t\t\t\tUse --help-effects for more detail"<<endl;
		cout<<endl;
		cout<<"  < {profile}\t\t\t\tSet a profile from a file (use --help-samples for more detail)"<<endl;
		cout<<"  |\t\t\t\t\tSet a profile from stdin (for scripting) (use --help-samples for more detail)"<<endl;
		cout<<endl;
		if((features | KeyboardFeatures::poweronfx) == features) {
			cout<<"  --startup-mode {startup mode}\t\tSet startup mode"<<endl;
			cout<<endl;
		}
		cout<<"  --list-keyboards \t\t\tList connected keyboards"<<endl;
		cout<<"  --print-device\t\t\tPrint device information for the keyboard"<<endl;
		cout<<endl;
		cout<<"  --help\t\t\t\tThis help"<<endl;
		cout<<"  --help-keys\t\t\t\tHelp for keys in groups"<<endl;
		cout<<"  --help-effects\t\t\tHelp for native effects"<<endl;
		cout<<"  --help-samples\t\t\tUsage samples"<<endl;
		cout<<endl;
		cout<<"Options:"<<endl;
		cout<<"  -dv\t\t\t\t\tDevice vendor ID, such as 046d for Logitech. Can be omitted to match any vendor ID"<<endl;
		cout<<"  -dp\t\t\t\t\tDevice product ID, such as c337 for Logitech G810. Can be omitted to match any product ID"<<endl;
		cout<<"  -ds\t\t\t\t\tDevice serial number, Can be omitted to match the first device found"<<endl;
		cout<<"  -tuk\t\t\t\t\tTest unsuported keyboard with one of supported protocol (1-3) -dv and -dp are required"<<endl;
		cout<<endl;
		cout<<"Values:"<<endl;
		if((features | KeyboardFeatures::rgb) == features)
			cout<<"  color formats :\t\t\tRRGGBB (hex value for red, green and blue)"<<endl;
		if((features | KeyboardFeatures::intensity) == features)
			cout<<"  color formats :\t\t\tII (hex value for intensity)"<<endl;
		if((features | KeyboardFeatures::setregion) == features)
			cout<<"  region formats :\t\t\tRN (integer value for region, 1 to 5)"<<endl;
		cout<<"  period formats :\t\t\tDms (decimal integer; units of milliseconds)"<<endl;
		cout<<"                  \t\t\tDs  (decimal integer; units of seconds)"<<endl;
		cout<<"                  \t\t\tSS  (hex value 01 to ff; units of 256ms)"<<endl;
		cout<<endl;
		if((features | KeyboardFeatures::setkey) == features)
			cout<<"  key values :\t\t\t\tabc... 123... and other (use --help-keys for more detail)"<<endl;
		if((features | KeyboardFeatures::setgroup) == features)
			cout<<"  group values :\t\t\tlogo, indicators, fkeys, ... (use --help-keys for more detail)"<<endl;
		cout<<"  startup mode :\t\t\twave, color"<<endl;
		cout<<endl;
	}
	
	// Need to check rgb and intesity
	void keys(char *arg0) {
		string cmdName = utils::getCmdName(arg0);
		KeyboardFeatures features = getKeyboardFeatures(cmdName);
		
		cout<<cmdName<<" Keys"<<endl;
		cout<<"-------------"<<endl;
		cout<<endl;
		cout<<"Group List :"<<endl;
		
		if((features | KeyboardFeatures::logo1) == features)
			cout<<"    logo"<<endl;
		if((features | KeyboardFeatures::setindicators) == features)
			cout<<"    indicators"<<endl;
		if((features | KeyboardFeatures::gkeys) == features)
			cout<<"    gkeys"<<endl;
		cout<<"    fkeys"<<endl;
		cout<<"    modifiers"<<endl;
		if((features | KeyboardFeatures::multimedia) == features)
			cout<<"    multimedia"<<endl;
		cout<<"    arrows"<<endl;
		if((features | KeyboardFeatures::numpad) == features)
			cout<<"    numeric"<<endl;
		cout<<"    functions"<<endl;
		cout<<"    keys"<<endl;
		cout<<endl;
		cout<<endl;
		
		if((features | KeyboardFeatures::logo1) == features) {
			cout<<"Group Logo :"<<endl;
			cout<<"    logo"<<endl;
			if((features | KeyboardFeatures::logo2) == features)
				cout<<"    logo2"<<endl;
			cout<<""<<endl;
		}
		
		if((features | KeyboardFeatures::setindicators) == features) {
			cout<<"Group indicators :"<<endl;
			cout<<"    num_indicator, numindicator, num"<<endl;
			cout<<"    caps_indicator, capsindicator, caps"<<endl;
			cout<<"    scroll_indicator, scrollindicator, scroll"<<endl;
			cout<<"    game_mode, gamemode, game"<<endl;
			cout<<"    back_light, backlight, light"<<endl;
			cout<<""<<endl;
		}
		
		if((features | KeyboardFeatures::gkeys) == features) {
			cout<<"Group gkeys :"<<endl;
			cout<<"    g1 - g9"<<endl;
			cout<<""<<endl;
		}
		
		cout<<"Group fkeys :"<<endl;
		cout<<"    f1 - f12"<<endl;
		cout<<""<<endl;
		
		cout<<"Group modifiers :"<<endl;
		cout<<"    shift_left, shiftleft, shiftl"<<endl;
		cout<<"    ctrl_left, ctrlleft, ctrll"<<endl;
		cout<<"    win_left, winleft, win_left"<<endl;
		cout<<"    alt_left, altleft, altl"<<endl;
		cout<<"    alt_right, altright, altr, altgr"<<endl;
		cout<<"    win_right, winright, winr"<<endl;
		cout<<"    menu"<<endl;
		cout<<"    ctrl_right, ctrlright, ctrlr"<<endl;
		cout<<"    shift_right, shiftright, shiftr"<<endl;
		cout<<""<<endl;
		
		if((features | KeyboardFeatures::multimedia) == features) {
			cout<<"Group multimedia :"<<endl;
			cout<<"    mute"<<endl;
			cout<<"    play_pause, playpause, play"<<endl;
			cout<<"    stop"<<endl;
			cout<<"    previous, prev"<<endl;
			cout<<"    next"<<endl;
			cout<<""<<endl;
		}
		
		cout<<"Group arrows :"<<endl;
		cout<<"    arrow_top, arrowtop, top"<<endl;
		cout<<"    arrow_left, arrowleft, left"<<endl;
		cout<<"    arrow_bottom, arrowbottom, bottom"<<endl;
		cout<<"    arrow_right, arrowright, right"<<endl;
		cout<<""<<endl;
		
		if((features | KeyboardFeatures::numpad) == features) {
			cout<<"Group numeric :"<<endl;
			cout<<"    num_lock, numlock"<<endl;
			cout<<"    num_slash, numslash, num/"<<endl;
			cout<<"    num_asterisk, numasterisk, num*"<<endl;
			cout<<"    num_minus, numminus, num-"<<endl;
			cout<<"    num_plus, numplus, num+"<<endl;
			cout<<"    numenter"<<endl;
			cout<<"    num0 - num9"<<endl;
			cout<<"    num_dot, numdot, num."<<endl;
			cout<<""<<endl;
		}
		
		cout<<"Group functions :"<<endl;
		cout<<"    escape, esc"<<endl;
		cout<<"    print_screen, printscreen, printscr"<<endl;
		cout<<"    scroll_lock, scrolllock"<<endl;
		cout<<"    pause_break, pausebreak"<<endl;
		cout<<"    insert, ins"<<endl;
		cout<<"    home"<<endl;
		cout<<"    page_up, pageup"<<endl;
		cout<<"    delete, del"<<endl;
		cout<<"    end"<<endl;
		cout<<"    page_down, pagedown"<<endl;
		cout<<""<<endl;
		
		if((features | KeyboardFeatures::setkey) == features) {
			cout<<"Group keys :"<<endl;
			cout<<"    0 - 9"<<endl;
			cout<<"    a - z"<<endl;
			cout<<"    tab"<<endl;
			cout<<"    caps_lock, capslock"<<endl;
			cout<<"    space"<<endl;
			cout<<"    backspace, back"<<endl;
			cout<<"    enter"<<endl;
			cout<<"    tilde"<<endl;
			cout<<"    minus"<<endl;
			cout<<"    equal"<<endl;
			cout<<"    open_bracket"<<endl;
			cout<<"    close_bracket"<<endl;
			cout<<"    backslash"<<endl;
			cout<<"    semicolon"<<endl;
			cout<<"    dollar"<<endl;
			cout<<"    quote"<<endl;
			cout<<"    intl_backslash"<<endl;
			cout<<"    comma"<<endl;
			cout<<"    period"<<endl;
			cout<<"    slash"<<endl;
		}
	}
	
	void effects(char *arg0) {
		string cmdName = utils::getCmdName(arg0);
		KeyboardFeatures features = getKeyboardFeatures(cmdName);
		cout<<cmdName<<" Effects"<<endl;
		cout<<"----------------"<<endl;
		cout<<endl;
		cout<<"At this time, FX are only tested on g512, g810, and gpro !"<<endl;
		cout<<endl;
		cout<<"  -fx ...      \t\t\t\tActivate an on-board lighting effect"<<endl;
		string optionalStore;
		if((features | KeyboardFeatures::userstoredlighting) == features) {
			cout<<"  -fx-store ...\t\t\t\tSet an on-board effect as user-stored lighting"<<endl;
			optionalStore = "[-store]";
		}
		cout<<endl;
		cout<<"  -fx" << optionalStore << " {effect} {target}"<<endl;
		cout<<endl;
		cout<<"  -fx" << optionalStore << " color {target} {color}"<<endl;
		cout<<"  -fx" << optionalStore << " breathing {target} {color} {period}"<<endl;
		cout<<"  -fx" << optionalStore << " cycle {target} {period}"<<endl;
		cout<<"  -fx" << optionalStore << " waves {target} {period}"<<endl;
		cout<<"  -fx" << optionalStore << " hwave {target} {period}"<<endl;
		cout<<"  -fx" << optionalStore << " vwave {target} {period}"<<endl;
		cout<<"  -fx" << optionalStore << " cwave {target} {period}"<<endl;
		cout<<endl;
		if((features | KeyboardFeatures::logo1) == features)
			cout<<"target value :\t\t\t\tall, keys, logo"<<endl;
		else
			cout<<"target value :\t\t\t\tall, keys (all is for compatibility with other keyboard models)"<<endl;
		if((features | KeyboardFeatures::rgb) == features)
			cout<<"color formats :\t\t\t\tRRGGBB (hex value for red, green and blue)"<<endl;
		else if((features | KeyboardFeatures::rgb) == features)
			cout<<"color formats :\t\t\t\tII (hex value for intensity)"<<endl;
		cout<<"period formats :\t\t\tDms (decimal integer; units of milliseconds)"<<endl;
		cout<<"                \t\t\tDs  (decimal integer; units of seconds)"<<endl;
		cout<<"                \t\t\tSS  (hex value 01 to ff; units of 256ms)"<<endl;
		cout<<endl;
	}
	
	// Need to check rgb and intesity
	void samples(char *arg0) {
		string cmdName = utils::getCmdName(arg0);
		KeyboardFeatures features = getKeyboardFeatures(cmdName);
		cout<<cmdName<<" Samples"<<endl;
		cout<<"----------------"<<endl;
		cout<<endl;
		cout<<"Samples :"<<endl;
		cout<<cmdName<<" -p /etc/g810/profile          # Load a profile"<<endl;
		if((features | KeyboardFeatures::setkey) == features)
			cout<<cmdName<<" -k logo ff0000                # Set color of a key"<<endl;
		if((features | KeyboardFeatures::setall) == features)
			cout<<cmdName<<" -a 00ff00                     # Set color of all keys"<<endl;
		if((features | KeyboardFeatures::setgroup) == features)
			cout<<cmdName<<" -g fkeys ff00ff               # Set color of a group of keys"<<endl;
		if((features | KeyboardFeatures::setregion) == features)
			cout<<cmdName<<" -r 1 ff0000                   # Set region 1 red"<<endl;
		cout<<cmdName<<" -fx color keys 00ff00         # Set fixed color effect"<<endl;
		cout<<cmdName<<" -fx breathing logo 00ff00 0a  # Set breathing effect"<<endl;
		cout<<cmdName<<" -fx cycle all 0a              # Set color cycle effect"<<endl;
		cout<<cmdName<<" -fx hwave keys 0a             # Set horizontal wave effect"<<endl;
		cout<<cmdName<<" -fx vwave keys 0a             # Set vertical wave effect"<<endl;
		cout<<cmdName<<" -fx cwave keys 0a             # Set center wave effect"<<endl;
		if((features | KeyboardFeatures::poweronfx) == features)
			cout<<cmdName<<" --startup-mode color          # Set keyboard power on effect"<<endl;
		cout<<endl;
		if((features | KeyboardFeatures::commit) == features) {
			cout<<"Samples with no commit :"<<endl;
			if((features | KeyboardFeatures::setall) == features)
				cout<<cmdName<<" -an 000000            # Set color of all key with no action"<<endl;
			if((features | KeyboardFeatures::setgroup) == features)
				cout<<cmdName<<" -gn modifiers ff0000  # Set color of a group with no action"<<endl;
			if((features | KeyboardFeatures::setkey) == features) {
				cout<<cmdName<<" -kn w ff0000          # Set color of a key with no action"<<endl;
				cout<<cmdName<<" -kn a ff0000          # Set color of a key with no action"<<endl;
				cout<<cmdName<<" -kn s ff0000          # Set color of a key with no action"<<endl;
				cout<<cmdName<<" -kn d ff0000          # Set color of a key with no action"<<endl;
			}
			cout<<cmdName<<" -c                    # Commit all changes"<<endl;
			cout<<endl;
		}
		// Need to be merged with Samples
		if((features | KeyboardFeatures::intensity) == features) {
			cout<<"Samples with intensity :"<<endl;
			cout<<cmdName<<" -a 60        # Set intensity of all keys"<<endl;
			cout<<cmdName<<" -k logo ff   # Set intensity of a key"<<endl;
			cout<<cmdName<<" -g fkeys aa  # Set intensity of a group of keys"<<endl;
			cout<<endl;
		}
		if((features | KeyboardFeatures::setkey) == features) {
			cout<<"Samples with pipe (for effects) :"<<endl;
			cout<<cmdName<<" -pp < profilefile # Load a profile"<<endl;
			cout<<"echo -e \"k w ff0000\\nk a ff0000\\nk s ff0000\\nk d ff0000\\nc\" | g810-led -pp # Set multiple keys"<<endl;
			cout<<endl;
		}
		cout<<"Testing an unsuported keyboard :"<<endl;
		cout<<"lsusb"<<endl;
		cout<<"#Sample result of lsusb : ID 046d:c331 Logitech, Inc. (dv=046d and dp=c331)"<<endl;
		cout<<cmdName<<" -dv 046d -dp c331 -tuk 1 -a 000000"<<endl;
		cout<<cmdName<<" -dv 046d -dp c331 -tuk 2 -a 000000"<<endl;
		cout<<cmdName<<" -dv 046d -dp c331 -tuk 3 -a 000000"<<endl;
		cout<<""<<endl;
	}
	
}
