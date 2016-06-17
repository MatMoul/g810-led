//#include <stdlib.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "classes/Keyboard.h"

using namespace std;


void usage() {
  cout<<"g810-led Usages :\n";
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
  cout<<"\n";
  cout<<"color formats :\t\tRRGGBB (hex value for red, green and blue)\n";
  cout<<"\n";
  cout<<"effect values :\t\trainbow, color\n";
  cout<<"key values :\t\tabc... 123... and other\n";
  cout<<"group values :\t\tlogo, indicators, fkeys, modifiers, multimedia, arrows, numeric, functions, keys\n";
  cout<<"\n";
  cout<<"sample :\n";
  cout<<"g810-led -k logo ff0000\n";
  cout<<"g810-led -a 00ff00\n";
  cout<<"g810-led -g fkeys ff00ff\n";
  cout<<"g810-led -s color\n";
}

int commit() {
  Keyboard g810;
  g810.attach();
  g810.commit();
  g810.detach();
  
  return 0;
}

int setStartupEffect(string effect) {
  Keyboard g810;
  Keyboard::PowerOnEffect powerOnEffect;
  if (g810.parsePowerOnEffect(effect, powerOnEffect) == true) {
    g810.attach();
    g810.setPowerOnEffect(powerOnEffect);
    g810.commit();
    g810.detach();
    //free(g810);
    //free(powerOnEffect);
    return 0;
  }
  delete[] g810;
  //delete powerOnEffect;
  return 1;
}

int setKey(string key, string color, bool commit) {
  Keyboard g810;
  Keyboard::KeyAddress keyAddress;
  if (g810.parseKey(key, keyAddress) == true) {
    Keyboard::KeyColors colors;
    if (g810.parseColor(color, colors) == true) {
      Keyboard::KeyValue keyValue;
      keyValue.key = keyAddress;
      keyValue.colors = colors;
      g810.attach();
      g810.setKey(keyValue);
      if (commit == true) g810.commit();
      g810.detach();
      return 0;
    }
  }
  return 1;
}

int setAllKeys(string color, bool commit) {
  Keyboard g810;
  Keyboard::KeyColors colors;
  if (g810.parseColor(color, colors) == true) {
    g810.attach();
    g810.setAllKeys(colors);
    if (commit == true) g810.commit();
    g810.detach();
    return 0;
  }
  return 1;
}

int setGroupKeys(string groupKeys, string color, bool commit) {
  Keyboard g810;
  Keyboard::KeyGroup keyGroup;
  if (g810.parseKeyGroup(groupKeys, keyGroup) == true) {
    Keyboard::KeyColors colors;
    if (g810.parseColor(color, colors) == true) {
      g810.attach();
      g810.setGroupKeys(keyGroup, colors);
      if (commit == true) g810.commit();
      g810.detach();
      //delete [] g810;
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
    bool commit;
    
    Keyboard g810;
    Keyboard::KeyGroup keyGroup;
    Keyboard::KeyAddress keyAddress;
    Keyboard::KeyValue keyValue;
    Keyboard::KeyColors colors;
    
    map<string, string> var;
    vector<Keyboard::KeyValue> keys;
    
    g810.attach();
    
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
        if (g810.parseColor(line, colors) == true) {
          keys.clear();
          g810.setAllKeys(colors);
        } else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
      } else if (line.substr(0,1) == "g") {
        line = line.substr(2);
        ind = line.find(" ");
        if (g810.parseKeyGroup(line.substr(0, ind), keyGroup) == true) {
          line = line.substr(ind + 1);
          if (line.substr(0, 1) == "$") {
            ind = line.find(" ");
            line = var[line.substr(1, ind - 1)];
          };
          if (g810.parseColor(line.substr(0, 6), colors) == true) {
            g810.setGroupKeys(keyGroup, colors);
          } else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
        } else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
      } else if (line.substr(0,1) == "k") {
        line = line.substr(2);
        ind = line.find(" ");
        if (g810.parseKey(line.substr(0, ind), keyAddress) == true) {
          line = line.substr(ind + 1);
          if (line.substr(0, 1) == "$") {
            ind = line.find(" ");
            line = var[line.substr(1, ind - 1)];
          }
          if (g810.parseColor(line.substr(0, 6), colors) == true) {
            keyValue.key = keyAddress;
            keyValue.colors = colors;
            keys.push_back(keyValue);
          } else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
        } else cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
      } else if (line.substr(0,1) == "c") {
        g810.commit();
        g810.setKeys(&keys[0], keys.size());
        keys.clear();
        g810.commit();
      } else if ((line.substr(0, 1) != "#") && (line.substr(0, 1) != "")) {
        cout<<"Error on line "<<lineCount<<" : "<<line<<"\n";
      }
    
      lineCount++;
    }
    
    g810.detach();
    
    file.close();
    
    return 0;
  }
  return 1;
}





int main(int argc, char *argv[]) {
  if (argc > 1) {
    string argCmd = argv[1];
    if (argCmd == "-h" || argCmd == "--help")       { usage(); return 0; }
    else if (argCmd == "-s" && argc == 3)           return setStartupEffect(argv[2]);
    else if (argCmd == "-a" && argc == 3)           return setAllKeys(argv[2], true);
    else if (argCmd == "-an" && argc == 3)          return setAllKeys(argv[2], false);
    else if (argCmd == "-g" && argc == 4)           return setGroupKeys(argv[2], argv[3], true);
    else if (argCmd == "-gn" && argc == 4)          return setGroupKeys(argv[2], argv[3], false);
    else if (argCmd == "-k" && argc == 4)           return setKey(argv[2], argv[3], true);
    else if (argCmd == "-kn" && argc == 4)          return setKey(argv[2], argv[3], false);
    else if (argCmd == "-c" && argc == 2)           return commit();
    else if (argCmd == "-p" && argc == 3)           return loadProfile(argv[2]);
  }
  usage();
  return 1;
}
