#include "help.h"

#include <iostream>
#include "utils.h"


using namespace std;

namespace help {
	
	string version = "0.1.1";
	
	void usage(char *arg0) {
		string cmdName = utils::getCmdName(arg0);
		cout<<cmdName<<endl;
		cout<<"--------"<<endl;
		cout<<"Version : "<<version<<endl;
		cout<<endl;
		cout<<"  -a {color}\t\t\t\tSet all keys color"<<endl;
		cout<<"  -g {keygroup} {color}\t\t\tSet key group color"<<endl;
		cout<<"  -k {key} {color}\t\t\tSet key color"<<endl;
		if (cmdName == "g910-led") {
			cout<<"  -mr {value}\t\t\t\tSet MR key (0-1)"<<endl;
			cout<<"  -mn {value}\t\t\t\tSet MN key (0-7) (M1=1, M2=2, M3=4) (M1+M2=3, M1+M3=5, ...)"<<endl;
			cout<<endl;
			cout<<"  -gkm {value}\t\t\t\tSet GKeys mode (0=Mapped to FKeys, 1=Independent)"<<endl;
		}
		cout<<endl;
		cout<<"  -an {color}\t\t\t\tSet all keys color without commit"<<endl;
		cout<<"  -gn {keygroup} {color}\t\tSet key group color without commit"<<endl;
		cout<<"  -kn {key} {color}\t\t\tSet key color without commit"<<endl;
		if (cmdName == "g910-led") {
			cout<<"  -mrn {value}\t\t\t\tSet MR key without commit"<<endl;
			cout<<"  -mnn {value}\t\t\t\tSet MN key without commit"<<endl;
		}
		cout<<"  -c\t\t\t\t\tCommit change"<<endl;
		cout<<endl;
		cout<<"  -fx ...\t\t\t\tUse --help-effects for more detail"<<endl;
		cout<<endl;
		cout<<"  < {profile}\t\t\t\tSet a profile from a file (use --help-samples for more detail)"<<endl;
		cout<<"  |\t\t\t\t\tSet a profile from stdin (for scripting) (use --help-samples for more detail)"<<endl;
		cout<<endl;
		cout<<"  --startup-mode {startup mode}\t\tSet startup mode"<<endl;
		cout<<endl;
		cout<<"  --list-keyboards \t\t\tList connected keyboards"<<endl;
		cout<<endl;
		cout<<"  --help\t\t\t\tThis help"<<endl;
		cout<<"  --help-keys\t\t\t\tHelp for keys in groups"<<endl;
		cout<<"  --help-effects\t\t\tHelp for native effects"<<endl;
		cout<<"  --help-samples\t\t\tUsage samples"<<endl;
		cout<<""<<endl;
		cout<<""<<endl;
		if (cmdName == "g610-led")
			cout<<"color formats :\t\t\t\tII (hex value for intensity)"<<endl;
		else
			cout<<"color formats :\t\t\t\tRRGGBB (hex value for red, green and blue)"<<endl;
		cout<<"speed formats :\t\t\t\tSS (hex value for speed 01 to ff)"<<endl;
		cout<<""<<endl;
		cout<<"key values :\t\t\t\tabc... 123... and other (use --help-keys for more detail)"<<endl;
		cout<<"group values :\t\t\t\tlogo, indicators, fkeys, ... (use --help-keys for more detail)"<<endl;
		cout<<"startup mode :\t\t\t\twave, color"<<endl;
		cout<<""<<endl;
	}
	
	void keys(char *arg0) {
		string cmdName = utils::getCmdName(arg0);
		cout<<cmdName<<" Keys"<<endl;
		cout<<"-------------"<<endl;
		cout<<endl;
		cout<<"Group List :"<<endl;
		
		if (cmdName == "g410-led") cout<<"    logo (Need confirmation)"<<endl; 
		else cout<<"    logo"<<endl; 
		if (cmdName != "g410-led") cout<<"    indicators"<<endl;
		if (cmdName == "g910-led") cout<<"    gkeys"<<endl;
		cout<<"    fkeys"<<endl;
		cout<<"    modifiers"<<endl;
		if (cmdName != "g410-led"    && cmdName != "g910-led") cout<<"    multimedia"<<endl;
		cout<<"    arrows"<<endl;
		if (cmdName != "g410-led") cout<<"    numeric"<<endl;
		cout<<"    functions"<<endl;
		cout<<"    keys"<<endl;
		cout<<endl;
		cout<<endl;
		
		// need confirmation for g410
		if (cmdName == "g410-led") {
			cout<<"Group logo :"<<endl;
			cout<<"    logo (Need confirmation)"<<endl;
			cout<<"    logo2 (Need confirmation)"<<endl;
		} else if (cmdName == "g910-led") {
			cout<<"Group logo :"<<endl;
			cout<<"    logo"<<endl;
			cout<<"    logo2"<<endl;
		} else {
			cout<<"Group logo :"<<endl;
			cout<<"    logo"<<endl;
		}
		cout<<""<<endl;
		
		if (cmdName != "g410-led") {
			cout<<"Group indicators :"<<endl;
			cout<<"    num_indicator, numindicator, num"<<endl;
			cout<<"    caps_indicator, capsindicator, caps"<<endl;
			cout<<"    scroll_indicator, scrollindicator, scroll"<<endl;
			cout<<"    game_mode, gamemode, game"<<endl;
			cout<<"    back_light, backlight, light"<<endl;
			cout<<""<<endl;
		}
		
		if (cmdName == "g910-led") {
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
		
		if (cmdName != "g410-led"    && cmdName != "g910-led") {
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
		
		if (cmdName != "g410-led") {
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
	
	void effects(char *arg0) {
		string cmdName = utils::getCmdName(arg0);
		cout<<cmdName<<" Effects"<<endl;
		cout<<"----------------"<<endl;
		cout<<endl;
		cout<<"At this time, FX are only tested on g810 !"<<endl;
		cout<<endl;
		cout<<"  -fx {effect} {target}"<<endl;
		cout<<endl;
		cout<<"  -fx color {target} {color}"<<endl;
		cout<<"  -fx breathing {target} {color} {speed}"<<endl;
		cout<<"  -fx cycle {target} {speed}"<<endl;
		cout<<"  -fx hwave {target} {speed}"<<endl;
		cout<<"  -fx vwave {target} {speed}"<<endl;
		cout<<"  -fx cwave {target} {speed}"<<endl;
		cout<<endl;
		cout<<"target value :\t\t\t\tall, keys, logo"<<endl;
		cout<<"color formats :\t\t\t\tRRGGBB (hex value for red, green and blue)"<<endl;
		cout<<"speed formats :\t\t\t\tSS (hex value for speed 01 to ff)"<<endl;
		cout<<endl;
	}
	
	void samples(char *arg0) {
		string cmdName = utils::getCmdName(arg0);
		cout<<cmdName<<" Samples"<<endl;
		cout<<"----------------"<<endl;
		cout<<endl;
		cout<<"Samples :"<<endl;
		cout<<"g810-led -p /etc/g810/profile          # Load a profile"<<endl;
		cout<<"g810-led -k logo ff0000                # Set color of a key"<<endl;
		cout<<"g810-led -a 00ff00                     # Set color of all keys"<<endl;
		cout<<"g810-led -g fkeys ff00ff               # Set color of a group of keys"<<endl;
		cout<<"g810-led -s color                      # Set keyboard power on effect"<<endl;
		cout<<"g810-led -fx color keys 00ff00         # Set fixed color effect"<<endl;
		cout<<"g810-led -fx breathing logo 00ff00 0a  # Set breathing effect"<<endl;
		cout<<"g810-led -fx cycle all 0a              # Set color cycle effect"<<endl;
		cout<<"g810-led -fx hwave keys 0a             # Set horizontal wave effect"<<endl;
		cout<<"g810-led -fx vwave keys 0a             # Set vertical wave effect"<<endl;
		cout<<"g810-led -fx cwave keys 0a             # Set center wave effect"<<endl;
		cout<<endl;
		cout<<"Samples with no commit :"<<endl;
		cout<<"g810-led -an 000000            # Set color of all key with no action"<<endl;
		cout<<"g810-led -gn modifiers ff0000  # Set color of a group with no action"<<endl;
		cout<<"g810-led -kn w ff0000          # Set color of a key with no action"<<endl;
		cout<<"g810-led -kn a ff0000          # Set color of a key with no action"<<endl;
		cout<<"g810-led -kn s ff0000          # Set color of a key with no action"<<endl;
		cout<<"g810-led -kn d ff0000          # Set color of a key with no action"<<endl;
		cout<<"g810-led -c                    # Commit all changes"<<endl;
		cout<<endl;
		cout<<"Samples for g610 :"<<endl;
		cout<<"g610-led -a 60        # Set intensity of all keys"<<endl;
		cout<<"g610-led -k logo ff   # Set intensity of a key"<<endl;
		cout<<"g610-led -g fkeys aa  # Set intensity of a group of keys"<<endl;
		cout<<endl;
		cout<<"Samples with pipe (for effects) :"<<endl;
		cout<<"g810-led -pp < profilefile # Load a profile"<<endl;
		cout<<"echo -e \"k w ff0000\\nk a ff0000\\nk s ff0000\\nk d ff0000\\nc\" | g810-led -pp # Set multiple keys"<<endl;
	}
	
}
