#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include "/usr/include/libusb-1.0/libusb.h"
#include "classes/Keyboard.h"

using namespace std;

int lg_pid;

void usage() {
  string appname = "g810-led";
  if (lg_pid == 0xc330) appname = "g410-led";
  cout<<appname<<" Usages :\n";
  cout<<"-----------------\n";
  cout<<"\n";
  cout<<"  -s  effect :\t\tSet keyboard startup effect\n";
  cout<<"\n";
  cout<<"  -a  color :\t\tSet all keys\n";
  cout<<"  -g  group, color :\tSet a group of keys\n";
  cout<<"  -k  key, color :\tSet a key\n";
  cout<<"\n";
  cout<<"  -an color :\t\tSet all keys without commit\n";
  cout<<"  -gn group, color :\tSet a group of keys without commit\n";
  cout<<"  -kn key, color :\tSet a key without commit\n";
  cout<<"\n";
  cout<<"  -c :\t\t\tCommit changes\n";
  cout<<"\n";
  cout<<"  -p  porfilefile :\tLoad a profile\n";
  cout<<"\n";
  cout<<"  -h | --help :\t\tthis help message\n";
  cout<<"  -lk | --list-keys :\tList keys in groups\n";
  cout<<"\n";
  cout<<"color formats :\t\tRRGGBB (hex value for red, green and blue)\n";
  cout<<"\n";
  cout<<"effect values :\t\trainbow, color\n";
  cout<<"key values :\t\tabc... 123... and other\n";
  cout<<"group values :\t\tlogo, indicators, fkeys, modifiers, multimedia, arrows, numeric, functions, keys\n";
  cout<<"\n";
  cout<<"sample :\n";
  cout<<appname<<" -k logo ff0000\n";
  cout<<appname<<" -a 00ff00\n";
  cout<<appname<<" -g fkeys ff00ff\n";
  cout<<appname<<" -s color\n";
}

void listkeys() {
  string appname = "g810-led";
  if (lg_pid == 0xc330) appname = "g410-led";
  cout<<appname<<" Keys in groups :\n";
  cout<<"-------------------------\n";
  cout<<"\n";
  cout<<"Group logo :\n";
  cout<<" Logo\n";
  cout<<"\n";
  cout<<"Group indicators :\n";
  cout<<" num_indicator, numindicator, num\n";
  cout<<" caps_indicator, capsindicator, caps\n";
  cout<<" scroll_indicator, scrollindicator, scroll\n";
  cout<<" game_mode, gamemode, game\n";
  cout<<" back_light, backlight, light\n";
  cout<<"\n";
  cout<<"Group fkeys :\n";
  cout<<" f1 - f12\n";
  cout<<"\n";
  cout<<"Group modifiers :\n";
  cout<<" shift_left, shiftleft, shiftl\n";
  cout<<" ctrl_left, ctrlleft, ctrll\n";
  cout<<" win_left, winleft, win_left\n";
  cout<<" alt_left, altleft, altl\n";
  cout<<" alt_right, altright, altr, altgr\n";
  cout<<" win_right, winright, winr\n";
  cout<<" menu\n";
  cout<<" ctrl_right, ctrlright, ctrlr\n";
  cout<<" shift_right, shiftright, shiftr\n";
  cout<<"\n";
  cout<<"Group multimedia :\n";
  cout<<" mute\n";
  cout<<" play_pause, playpause, play\n";
  cout<<" stop\n";
  cout<<" previous, prev\n";
  cout<<" next\n";
  cout<<"\n";
  cout<<"Group arrows :\n";
  cout<<" arrow_top, arrowtop, top\n";
  cout<<" arrow_left, arrowleft, left\n";
  cout<<" arrow_bottom, arrowbottom, bottom\n";
  cout<<" arrow_right, arrowright, right\n";
  cout<<"\n";
  cout<<"Group numeric :\n";
  cout<<" num_lock, numlock\n";
  cout<<" num_slash, numslash, num/\n";
  cout<<" num_asterisk, numasterisk, num*\n";
  cout<<" num_minus, numminus, num-\n";
  cout<<" num_plus, numplus, num+\n";
  cout<<" numenter\n";
  cout<<" num0 - num9\n";
  cout<<" num_dot, numdot, num.\n";
  cout<<"\n";
  cout<<"Group functions :\n";
  cout<<" escape, esc\n";
  cout<<" print_screen, printscreen, printscr\n";
  cout<<" scroll_lock, scrolllock\n";
  cout<<" pause_break, pausebreak\n";
  cout<<" insert, ins\n";
  cout<<" home\n";
  cout<<" page_up, pageup\n";
  cout<<" delete, del\n";
  cout<<" end\n";
  cout<<" page_down, pagedown\n";
  cout<<"\n";
  cout<<"Group keys :\n";
  cout<<" 0 - 9\n";
  cout<<" a - z\n";
  cout<<" tab\n";
  cout<<" caps_lock, capslock\n";
  cout<<" space\n";
  cout<<" backspace, back\n";
  cout<<" enter\n";
  cout<<" tilde\n";
  cout<<" minus\n";
  cout<<" equal\n";
  cout<<" open_bracket\n";
  cout<<" close_bracket\n";
  cout<<" backslash\n";
  cout<<" semicolon\n";
  cout<<" dollar\n";
  cout<<" quote\n";
  cout<<" intl_backslash\n";
  cout<<" comma\n";
  cout<<" period\n";
  cout<<" slash\n";
}

int commit() {
  Keyboard lg_kbd;
  lg_kbd.attach(lg_pid);
  lg_kbd.commit();
  lg_kbd.detach();
  
  return 0;
}

int setStartupEffect(string effect) {
  Keyboard lg_kbd;
  Keyboard::PowerOnEffect powerOnEffect;
  if (lg_kbd.parsePowerOnEffect(effect, powerOnEffect) == true) {
    lg_kbd.attach(lg_pid);
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
      lg_kbd.attach(lg_pid);
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
    lg_kbd.attach(lg_pid);
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
      lg_kbd.attach(lg_pid);
      lg_kbd.setGroupKeys(keyGroup, colors);
      if (commit == true) lg_kbd.commit();
      lg_kbd.detach();
      return 0;
    }
  }
  return 1;
}

int loadProfile(string profileFile) {
  ifstream file;
  
  file.open(profileFile);
  if (file.is_open()) {
    
    string line;
    int lineCount = 1;
    int ind;
    
    Keyboard lg_kbd;
    Keyboard::KeyGroup keyGroup;
    Keyboard::KeyAddress keyAddress;
    Keyboard::KeyValue keyValue;
    Keyboard::KeyColors colors;
    
    map<string, string> var;
    vector<Keyboard::KeyValue> keys;
    
    lg_kbd.attach(lg_pid);
    
    while (!file.eof()) {
      getline(file, line);
      
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
      } else if ((line.substr(0, 1) != "#") && (line.substr(0, 1) != "")) {
        cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
      }
    
      lineCount++;
    }
    
    lg_kbd.detach();
    
    file.close();
    
    return 0;
  }
  return 1;
}


void findG810() {
  libusb_device **devs;
  libusb_context *ctx = NULL;
  int r;
  ssize_t cnt;
  r = libusb_init(&ctx);
  if(r < 0) return;
  cnt = libusb_get_device_list(ctx, &devs);
  if(cnt < 0) return;
  ssize_t i;
  for(i = 0; i < cnt; i++) {
    libusb_device *device = devs[i];
    libusb_device_descriptor desc = {0};
    libusb_get_device_descriptor(device, &desc);
    if (desc.idVendor == 0x046d) {
      if (desc.idProduct == 0xc331) { lg_pid=0xc331; break; }
      if (desc.idProduct == 0xc337) { lg_pid=0xc337; break; }
    }
  }
  libusb_free_device_list(devs, 1); 
  libusb_exit(ctx);
}



int main(int argc, char *argv[]) {
  string str = argv[0];
  size_t split = str.find_last_of("/\\");
  str = str.substr(split + 1);
  
  if (str == "g410-led") lg_pid=0xc330;
  else findG810();
  
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
  }
  usage();
  return 1;
}
